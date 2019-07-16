//*************************************************************************
//
// Program : sixdofrunner.m
// Description : Run sixdof with single mode or batch mode
//
//*************************************************************************
//
// Revision record : 2019 / 06 / 14 H.A(Generated)
//				   : 2019 / 06 /    W.M(Convert)
//
//*************************************************************************

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

void main() {
	std::ofstream ofs("sixdoflog.txt");

	//****************** Global parameters*******************
	//==================== Units converter====================
	double pi = 4 * atan(1.0);
	double DG2RD = pi / 180;// deg �� rad
	double RD2DG = 180 / pi;// rad �� deg
	double FT2ME = 0.30480371;// ft  �� m
	double ME2FT = 1 / 0.30480371;// m   �� ft
	double KT2MS = 0.5144;// kt  �� m / s
	double MS2KT = 1 / 0.5144;// ms  �� kt
	//=============== Control surface Limitter =================
	double dh_max = 10; dh_min = -25;// Max& Min dh deflection
	double da_max = 15; da_min = -15;// Max& Min da deflection
	double dr_max = 30; dr_min = -30;// Max& Min dr deflection
	double Ct_min = 0;// Minimum Thrust coeff
	//================ Init value for trim cal =================
	double Ct_trim = 0;// Initial trim thrust coeff
	double alpha_trim = 0;// Initial trim angle of attack
	double beta_trim = 0;// Initial trim angle of sideslip
	double theta_trim = 0;// Initial trim pitch angle
	double dh_trim = 0;// Initial trim dh deflection
	double da_trim = 0;// Initial trim da deflection
	double dr_trim = 0;// Initial trim dr deflection
	double phid_trim = 0;// Initial bank change rate
	double thetad_trim = 0;// Initial pitch change rate
	double psid_trim = 0;// initial heading change tare
	//====================== Trim params =======================
	double maxIter_trim = 1.0e5;// Iteration limit
	double tol_trim = 1.0e-11;// Tolerance
	double cv_flag = 0;
	//=============== Simulation running params ================
	double ds = 0.02;// Simulation time steps
	double t_simu = 90;// Maximum of simulation time
	//= ============== Program Entry Point Flags ===============
	//global Block0 Block1 Block2 Block3
	// ********************************************************


	ofs.close();
}