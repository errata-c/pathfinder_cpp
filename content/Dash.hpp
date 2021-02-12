#pragma once
#include "Outline.hpp"

namespace pf {
	struct DashState {


		bool isOn() const noexcept;

		Contour output;
		std::size_t currentIndex;
		float distanceLeft;
	};

	struct Dash {
		Outline output;

	};

	struct OutlineDash {
		Outline output;
		DashState state;
	};

	struct ContourDash {

	};

	
};