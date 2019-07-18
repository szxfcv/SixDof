//*************************************************************************
//
// Program : sixdofrunner.m
// Description : Run sixdof with single mode or batch mode
//
//*************************************************************************
//
// Revision record : 2019 / 06 / 14 H.A(Generated)
//				   : 2019 / 07 /    W.M(Convert)
//
//*************************************************************************

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>


std::vector<std::string> split(std::string str, char del) {
	int first = 0;
	int last = str.find_first_of(del);

	std::vector<std::string> result;

	while (first < str.size()) {
		std::string subStr(str, first, last - first);

		result.push_back(subStr);

		first = last + 1;
		last = str.find_first_of(del, first);

		if (last == std::string::npos) {
			last = str.size();
		}
	}
	return result;
}


int main() {
	std::ofstream ofs("sixdoflog.txt");

	//****************** Global parameters *********************
	//==================== Units converter =====================
	double pi = 4 * atan(1.0);
	double DG2RD = pi / 180;		// deg ⇒ rad
	double RD2DG = 180 / pi;		// rad ⇒ deg
	double FT2ME = 0.30480371;		// ft  ⇒ m
	double ME2FT = 1 / 0.30480371;	// m   ⇒ ft
	double KT2MS = 0.5144;			// kt  ⇒ m / s
	double MS2KT = 1 / 0.5144;		// ms  ⇒ kt
	//=============== Control surface Limitter =================
	double dh_max = 10;				// Max& Min dh deflection
	double dh_min = -25;
	double da_max = 15;				// Max& Min da deflection
	double da_min = -15;
	double dr_max = 30;				// Max& Min dr deflection
	double dr_min = -30;
	double Ct_min = 0;				// Minimum Thrust coeff
	//================ Init value for trim cal =================
	double Ct_trim = 0;				// Initial trim thrust coeff
	double alpha_trim = 0;			// Initial trim angle of attack
	double beta_trim = 0;			// Initial trim angle of sideslip
	double theta_trim = 0;			// Initial trim pitch angle
	double dh_trim = 0;				// Initial trim dh deflection
	double da_trim = 0;				// Initial trim da deflection
	double dr_trim = 0;				// Initial trim dr deflection
	double phid_trim = 0;			// Initial bank change rate
	double thetad_trim = 0;			// Initial pitch change rate
	double psid_trim = 0;			// initial heading change tare
	//====================== Trim params =======================
	double maxIter_trim = 1.0e5;	// Iteration limit
	double tol_trim = 1.0e-11;		// Tolerance
	double cv_flag = 0;
	//=============== Simulation running params ================
	double ds = 0.02;				// Simulation time steps
	double t_simu = 90;				// Maximum of simulation time
	//= ============== Program Entry Point Flags ===============
	//global Block0 Block1 Block2 Block3
	// *********************************************************
	// Disp program title
	std::cout << "*******************************************************************" << std::endl;
	std::cout << "                                                                   " << std::endl;
	std::cout << "   Six-DOF Aircraft simulation program                             " << std::endl;
	std::cout << "                                                                   " << std::endl;
	std::cout << "*******************************************************************" << std::endl;
	std::cout << "                                                                   " << std::endl;
	std::cout << " <Status>                                                          " << std::endl;
	std::cout << "  2019/05/30 Add trim flight simulation logics (A. H)              " << std::endl;
	std::cout << "  2019/06/04 Add time response analysis logic  (A. H)              " << std::endl;
	std::cout << "  2019/06/26 Add batch simulation program      (A. H)              " << std::endl;
	std::cout << "  2019/07/04 Add Steady wind enviroment logic  (A. H)              " << std::endl;
	std::cout << "  2019/07/16~Convert Matlab to C++             (M. W)              " << std::endl;
	std::cout << "                                                                   " << std::endl;
	std::cout << "*******************************************************************" << std::endl;

	//--- Simulation flags ------------------------------------
	//Computation mode flag
	std::string singlemode("off");
	std::string batchmode("run");

	std::cout << "singlemode is " << singlemode << std::endl;
	std::cout << "batchmode is " << batchmode << std::endl;

	//--- Phase flag ------------------------------------------
	// --------------------------------------------------------
	//
	// Default ⇒　'run '
	// (If you don't need of them, change 'run' to 'off')
	//
	//---------------------------------------------------------
	std::string Block0("run");		// Mode selector
	std::string Block1("run");		// Get simulation parameters
	std::string Block2("run");		// Trim cal.
	std::string Block3("run");		// Time response simulation
	// ---------------------------------------------------------

	//--- File import ------------------------------------------
	std::ifstream fileID_05("Flightcondition.txt");
	const int buf_size = 211;
	char str[buf_size];
	if(fileID_05.fail()){
		std::cerr << "Failed to open file." << std::endl;
		return -1;
	}

	fileID_05.getline(str,buf_size);
	std::string FCHead = str;
	std::string f_condition_mat[100];
	int case_num = 0;				// Number of simulation case
	while (fileID_05.getline(str, buf_size)) {
		f_condition_mat[case_num] = str;
		case_num++;
	}
	fileID_05.close();
	std::cout << "FCHead is " << FCHead << std::endl;
	std::cout << "case_num is " << case_num << std::endl;
//std::cout << "f_condition_mat is " << f_condition_mat[0] << std::endl;
	//Restab_n = zeros(case_num, 17);
	std::vector<std::vector<int> > Restab_n(case_num, std::vector<int>(17,0));

	// 入力 (2重ループを用いる)
	for (int i = 0; i < case_num; i++) {
		for (int j = 0; j < 17; j++) {
			std::cout << Restab_n.at(i).at(j);
		}
		std::cout << std::endl;
	}
//graveyard
/*		char del = ',';
		for (const auto substr : split(f_condition_mat[i], del)) {
			std::cout << "substr is " << substr << std::endl;
			Restab_n.at(i).at(j) = std::stoi(substr);
			std::cout << "j is " << j << std::endl;
			j++;
		}
		//Restab_n.at(i).at(j) = std::stoi(split(f_condition_mat[i], del)[i]);
		//Restab_n.at(i) = split(f_condition_mat[i], del);
		//std::cout << Restab_n.at(i).at(j) << " ";
		std::cout << std::endl;
	}*/
	

	ofs.close();
	return 0;
}