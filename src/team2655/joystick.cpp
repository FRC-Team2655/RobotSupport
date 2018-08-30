#include "joystick.hpp"

/// Adapted from the gist https://gist.github.com/chrisengelsma/108f7ab0a746323beaaf7d6634cf4add
template <class TYPE>
bool team2655::jshelper::polyfit(const std::vector<TYPE> & x, const std::vector<TYPE> & y, const int &order, std::vector<TYPE> &coeffs) {
    // The size of xValues and yValues should be same
	if (x.size() != y.size()) {
		throw std::runtime_error( "Polyfit cannot work with different x and y array sizes!" );
		return false;
	}
	// The size of xValues and yValues cannot be 0, should not happen
	if (x.size() == 0 || y.size() == 0) {
		throw std::runtime_error( "Polyfit cannot work with x or y arrays with a size of 0!" );
		return false;
	}

	size_t N = x.size();
	int n = order;
	int np1 = n + 1;
	int np2 = n + 2;
	int tnp1 = 2 * n + 1;
	TYPE tmp;

	// X = vector that stores values of sigma(xi^2n)
	std::vector<TYPE> X(tnp1);
	for (int i = 0; i < tnp1; ++i) {
		X[i] = 0;
		for (size_t j = 0; j < N; ++j)
			X[i] += (TYPE)pow(x[j], i);
	}

	// a = vector to store final coefficients.
	std::vector<TYPE> a(np1);

	// B = normal augmented matrix that stores the equations.
	std::vector<std::vector<TYPE> > B(np1, std::vector<TYPE> (np2, 0));

	for (int i = 0; i <= n; ++i)
		for (int j = 0; j <= n; ++j)
			B[i][j] = X[i + j];

	// Y = vector to store values of sigma(xi^n * yi)
	std::vector<TYPE> Y(np1);
	for (int i = 0; i < np1; ++i) {
		Y[i] = (TYPE)0;
		for (size_t j = 0; j < N; ++j) {
			Y[i] += (TYPE)pow(x[j], i)*y[j];
		}
	}

	// Load values of Y as last column of B
	for (int i = 0; i <= n; ++i)
		B[i][np1] = Y[i];

	n += 1;
	int nm1 = n-1;

	// Pivotisation of the B matrix.
	for (int i = 0; i < n; ++i)
		for (int k = i+1; k < n; ++k)
			if (B[i][i] < B[k][i])
				for (int j = 0; j <= n; ++j) {
					tmp = B[i][j];
					B[i][j] = B[k][j];
					B[k][j] = tmp;
				}

	// Performs the Gaussian elimination.
	// (1) Make all elements below the pivot equals to zero
	//     or eliminate the variable.
	for (int i=0; i<nm1; ++i)
		for (int k =i+1; k<n; ++k) {
			TYPE t = B[k][i] / B[i][i];
			for (int j=0; j<=n; ++j)
				B[k][j] -= t*B[i][j];         // (1)
	}

	// Back substitution.
	// (1) Set the variable as the rhs of last equation
	// (2) Subtract all lhs values except the target coefficient.
	// (3) Divide rhs by coefficient of variable being calculated.
	for (int i=nm1; i >= 0; --i) {
		a[i] = B[i][n];                   // (1)
		for (int j = 0; j<n; ++j)
			if (j != i)
				a[i] -= B[i][j] * a[j];       // (2)
		a[i] /= B[i][i];                  // (3)
	}

	coeffs.resize(a.size());
	for (size_t i = 0; i < a.size(); ++i)
		coeffs.insert(coeffs.begin() + i, a[i]);

	return true;
}

team2655::jshelper::AxisConfig team2655::jshelper::createAxisConfig(double deadband, double minPower, double midPower){

	// NO NEGATIVE VALUES!!! The regression is generated for the 1st quadrant. If the input is negative the output will be negated.
	deadband = fabs(deadband);
	minPower = fabs(minPower);
	midPower = fabs(midPower);

	double midDeadband = (1 - deadband) / 2 + deadband; // Middle of deadband and 1

	// The smallest x with a non-zero value, the middle position, the middle position + a tiny bit (so flat part of cubic is here), the max x value (1)
	std::vector<double> xCoords = { deadband, midDeadband, midDeadband + 0.01, 1 };

	// The minimum value, The mid power, the mid power (for the flat part), the max y value (1)
	std::vector<double> yCoords = {minPower, midPower, midPower, 1};

	std::array<double, 5> coefficients;
	std::vector<double> results;

	// Get the coordinates
	if(polyfit<double>(xCoords, yCoords, 3, results)){
		std::copy_n(results.begin(), 4, coefficients.begin());
		coefficients[4] = deadband;
	}else{
		// If generating the regression fails: default to a linear function that will only apply the jshelper deadband.
		createAxisConfig(deadband);
	}
	return coefficients; // This is in the order {d, c, b, a, deadband} where f(x)=ax^3+bx^2+cx+d with x as the joystick input
}

team2655::jshelper::AxisConfig team2655::jshelper::createAxisConfig(double deadband){
	deadband = fabs(deadband);
	return std::array<double, 5>{ 0, 1, 0, 0, deadband }; // This is in the order {d, c, b, a, deadband} where f(x)=ax^3+bx^2+cx+d with x as the joystick input
}

double team2655::jshelper::getAxisValue(const team2655::jshelper::AxisConfig config, const double axisValue, bool deadbandOnly){

	// Adhere to the set deadband
	if(fabs(axisValue) < config[4]){
		return 0;
	}

	// Check if this is a linear relationship (if so deadband needs to be applied differently to avoid a "jump" when passing the deadband threshold
	// If the user requested deadband application only also treat this as linear.
	// Coefficients contains the cubic functions coefficients (indices 0-3) and the deadband (index 4)
	if(deadbandOnly || (config[0] == 0 && config[1] == 1 && config[2] == 0 && config[3] == 0)){
		// This is linear. Only need to apply a deadband.
		// This will scale the value up after the deadband. Ex. if deadband is 0.1 this will make x=0.1 return y=0 instead of y=0.1 (the jump)
		return (axisValue - (fabs(axisValue) / axisValue * config[4])) / (1 - config[4]); // This will do the scaling
	}else{
		// Do everything in the first quadrant (+x, +y) then move to third quadrant if x is (-)
		double x = fabs(axisValue);

		// Do the calculation
		double result = config[3] * pow(x, 3) + config[2] * pow(x, 2) + config[1] * x + config[0];

		// Apply the correct sign
		if((axisValue < 0 && result > 0) || (axisValue > 0 && result < 0))
			result *= -1;
		return result;
	}
}

