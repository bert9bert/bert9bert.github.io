/** Code for blog post at 
	http://blog.bertramieong.com/math%20stat/c/simulation/2014/12/14/draw-beta-two-ways.html **/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

int compare_doubles(const void *x, const void *y) {
	double a = *((double*)x);
	double b = *((double*)y);
	if(a > b) return  1;
	if(a < b) return -1;
	return 0;
}

double draw_standard_uniform() {
	/* Draw random number from uniform distribution on (0,1] */
	int x = (double) rand()/(double)RAND_MAX;
	return (x + (1/RAND_MAX))/(1+(1/RAND_MAX));
}

double draw_beta_intparm_ordermethod(int alpha, int beta) {
	/* Draw from a Beta distribution with integer parameters
	using the order statistics method */

	int i;
	double draw;
	int num_unif_draws = alpha + beta - 1;
	double *unif_draws;
	unif_draws = (double*) malloc(num_unif_draws * sizeof(double));
	if(!unif_draws) return 1;

	for (i=0; i<num_unif_draws; i++) {
		unif_draws[i] = draw_standard_uniform();
	}

	qsort(unif_draws, num_unif_draws, sizeof(unif_draws[0]), compare_doubles);
	draw = unif_draws[alpha-1];

	free(unif_draws);
	return draw;
}

double draw_beta_intparm_gammamethod(int alpha, int beta) {
	/* Draw from a Beta distribution with integer parameters
	using the function of Gamma random variables method */

	int i;
	double gamma_alpha_1, gamma_beta_1;
	double draw;

	gamma_alpha_1 = 0;
	for(i=0; i<alpha; i++) {
		gamma_alpha_1 += -log(draw_standard_uniform());
	}

	gamma_beta_1 = 0;
	for(i=0; i<beta; i++) {
		gamma_beta_1 += -log(draw_standard_uniform());
	}

	draw = gamma_alpha_1/(gamma_alpha_1+gamma_beta_1);

	return draw;
}

double expertime_draw_beta(int alpha, int beta, int method, int num_draws) {
	/* Perform a number of draws from a Beta distribution with integer parameters
	and return the time needed to perform all the draws */

	int i;
	double TOL;
	clock_t begin, end;
	double time_spent;
	double beta_draws_mean, beta_actual_mean, check_diff;
	double *beta_draws;

	beta_draws = (double*) malloc(num_draws * sizeof(double));
	if(!beta_draws){
		printf("ERROR\n\n");
		return -1;
	}

	if(method==1){  // order method
		begin = clock();
		for(i=0; i<num_draws; i++) {
			beta_draws[i] = draw_beta_intparm_ordermethod(alpha,beta);
		}
		end = clock();

	} else if(method==2){  // gamma method
		begin = clock();
		for(i=0; i<num_draws; i++) {
			beta_draws[i]   = draw_beta_intparm_gammamethod(alpha,beta);
		}
		end = clock();
	} else {  // invalid method
		printf("ERROR\n\n");
		return -1;
	}
	
	/** check that the simulated mean is close to the actual mean 
	if the number of draws is sufficiently high**/
	TOL = 0.005;
	beta_draws_mean = 0;
	for(i=0; i<num_draws; i++) {
		beta_draws_mean += beta_draws[i];
	}
	beta_draws_mean = beta_draws_mean/num_draws;
	beta_actual_mean = (double) alpha/(alpha+beta);
	check_diff = abs(beta_draws_mean - beta_actual_mean);
	if((check_diff>=TOL) && ((alpha+beta)>100)) {
		printf("ERROR: simulated mean is far from actual mean in Beta draws. \n\n");
		printf("alpha = %d, beta = %d, method = %d\n", alpha, beta, method);
		printf("Simulated Mean = %f, Actual Mean = %f, Diff = %f\n", 
			beta_draws_mean, beta_actual_mean, check_diff);
		printf("Run this program again and see if the problem re-appears.\n");
		return -1;
	}

	time_spent = (double)(end-begin)/CLOCKS_PER_SEC;
	return time_spent;
}

int main(int argc, char *argv[]) {
	const int num_draws = 10000;
	int run_with_pauses = 0;
	int i;
	int alphavec1[111], betavec1[111], alphavec2[61], betavec2[61];
	double time_spent_11vec[111], time_spent_21vec[111];  // times using one-sided (1st) set of parameters
	double time_spent_12vec[61], time_spent_22vec[61];  // times using symmetric (2nd) set of parameters
	
	/** determine if program should be run with pauses that
	prompt the user to continue **/
	if(argc>1){
		if(!strcmp(argv[1], "-pauses")) {
			run_with_pauses = 1;
		}
	}

	/** Create parameter sets to simulate on **/
	/* one-sided set */
	for(i=1; i<=100; i++) {
		alphavec1[i-1] = 1;
		betavec1[i-1] = i;
	}
	for(i=101; i<=111; i++) {
		alphavec1[i-1] = 1;
	}
	betavec1[101-1] = 500;
	for(i=102; i<=111; i++) {
		betavec1[i-1] = (i-101)*1000;
	}

	/* symmetric set */
	for(i=1; i<=50; i++) {
		alphavec2[i-1] = i;
		betavec2[i-1] = i;
	}
	alphavec2[51-1] = 250;
	betavec2[51-1] = 250;
	for(i=52; i<=61; i++) {
		alphavec2[i-1] = (i-52+1)*1000/2;
		betavec2[i-1] = (i-52+1)*1000/2;
	}


	/** set seed for random number generator **/
	srand(time(NULL));

	/** run simulations of Beta draws with the two different drawing methods, and two
	different parameter sets **/
	/* one-sided parameter set */
	for(i=0; i<111; i++){
		time_spent_11vec[i] = expertime_draw_beta(alphavec1[i], betavec1[i], 1, num_draws);
	}

	for(i=0; i<111; i++){
		time_spent_21vec[i] = expertime_draw_beta(alphavec1[i], betavec1[i], 2, num_draws);
	}

	/* symmetric parameter set */
	for(i=0; i<61; i++){
		time_spent_12vec[i] = expertime_draw_beta(alphavec2[i], betavec2[i], 1, num_draws);
	}

	for(i=0; i<61; i++){
		time_spent_22vec[i] = expertime_draw_beta(alphavec2[i], betavec2[i], 2, num_draws);
	}

	/** output results **/
	if(run_with_pauses) {
		printf("Press ENTER to continue...\n");
		getchar();
	}

	/* one-sided parameter set */
	// method 1: order statistics method
	printf("Note: time is for %d total draws\n\n", num_draws);
	printf("alpha\tbeta\ttime\tmethod\tparam_set\n");
	for(i=0; i<111; i++){
		printf("%5d %5d %10.6f %5d %5d\n", alphavec1[i], betavec1[i], time_spent_11vec[i], 1, 1);
	}

	// method 2: gamma draws method
	for(i=0; i<111; i++){
		printf("%5d %5d %10.6f %5d %5d\n", alphavec1[i], betavec1[i], time_spent_21vec[i], 2, 1);
	}

	/* symmetric parameter set */
	if(run_with_pauses) {
		printf("Press ENTER to continue...\n");
		getchar();
	}

	// method 1: order statistics method
	for(i=0; i<61; i++){
		printf("%5d %5d %10.6f %5d %5d\n", alphavec2[i], betavec2[i], time_spent_12vec[i], 1, 2);
	}

	// method 2: gamma draws method
	for(i=0; i<61; i++){
		printf("%5d %5d %10.6f %5d %5d\n", alphavec2[i], betavec2[i], time_spent_22vec[i], 2, 2);
	}


	return 0;
}

