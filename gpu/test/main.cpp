#include <fmt/core.h>
#include "../half.hpp"



void printBits(float val) {
	union Convert {
		uint32_t ival;
		float val;
	};

	Convert convert;
	convert.val = val;
	for (int i = 31; i != -1; --i) {
		if (convert.ival & (1u << i)) {
			fmt::print("1");
		}
		else {
			fmt::print("0");
		}

		if (i == 31 || i == 23) {
			fmt::print(" ");
		}
	}
	fmt::print("\n");
}

void printBits(pf::half val) {
	union Convert {
		Convert() {};

		uint16_t ival;
		pf::half val;
	};

	Convert convert;
	convert.val = val;
	for (int i = 15; i != -1; --i) {
		if (convert.ival & (1u << i)) {
			fmt::print("1");
		}
		else {
			fmt::print("0");
		}

		if (i == 15 || i == 10) {
			fmt::print(" ");
		}
	}
	fmt::print("\n");
}

void printBits(uint32_t val) {
	for (int i = 31; i != -1; --i) {
		if (val & (1u << i)) {
			fmt::print("1");
		}
		else {
			fmt::print("0");
		}

		if (i == 31 || i == 23) {
			fmt::print(" ");
		}
	}
	fmt::print("\n");
}

int main(int argc, char * argv[]) {
	float a = 1.f;
	float b = 0.125f;
	float c = 0.0625f;
	float pi = 3.141592653;

	fmt::print("Single precision:\n");
	printBits(a);
	printBits(b);
	printBits(c);
	printBits(pi);
	fmt::print("\n");

	fmt::print("\nHalf precision:\n");
	pf::half ha{ a }, hb{ b }, hc{ c }, hpi{ pi };
	printBits(ha);
	printBits(hb);
	printBits(hc);
	printBits(hpi);
	fmt::print("\n");

	fmt::print("Reconstructed single precision:\n");
	float pa = static_cast<float>(ha);
	float pb = static_cast<float>(hb);
	float pc = static_cast<float>(hc);
	float ppi = static_cast<float>(hpi);
	printBits(pa);
	printBits(pb);
	printBits(pc);
	printBits(ppi);
	fmt::print("\n");

	fmt::print("{}\n", pa);
	fmt::print("{}\n", pb);
	fmt::print("{}\n", pc);
	fmt::print("{}\n", ppi);

	fmt::print("\n");
	uint32_t sign = 0x8000,
		exponent = 0x7C00,
		mantissa = 0x3FF,
		mval = 0x70;

	// sign mask     8000
	// exponent mask 3E00
	// mantissa mask  1FF

	printBits(sign);
	printBits(exponent);
	printBits(mantissa);

	uint32_t
		offset_sign = sign << 16,
		offset_exponent = exponent << 13,
		offset_mantissa = mantissa << 13;

	fmt::print("\n");
	printBits(offset_sign);
	printBits(offset_exponent);
	printBits(offset_mantissa);
	printBits(mval << 10);
}