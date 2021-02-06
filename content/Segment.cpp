#include "Segment.hpp"

#include <cmath>

static const float root2 = std::sqrt(2.f);
static const float eps = 1E-4f;

/*
vec2 eval_horner(t) { 
	u = 1 - t; bc = 1; tn = 1; 
	tmp = controlpoints[0] * u; 
	for (int i = 1; i < n; i++) { 
		tn *= t; bc *= (n - i + 1) / i; 
		tmp = (tmp + tn * bc * controlpoints) * u; 
	} 
	return (tmp + tn * t * controlpoints[n]); 
}
*/