//// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= ////
//// Mathematics utils for c++ game programming.
////
//// MIT Licanse
////
//// Copyright (c) 2022 Mikko Romppainen.
////
//// Permission is hereby granted, free of charge, to any person obtaining
//// a copy of this software and associated documentation files (the
//// "Software"), to deal in the Software without restriction, including
//// without limitation the rights to use, copy, modify, merge, publish,
//// distribute, sublicense, and/or sell copies of the Software, and to
//// permit persons to whom the Software is furnished to do so, subject to
//// the following conditions:
////
//// The above copyright notice and this permission notice shall be included
//// in all copies or substantial portions of the Software.
////
//// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= ////
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <complex>

namespace math {
	template<typename V>
	V add(const V& a, const V& b) {
		V res = a;
		for(size_t i=0; i<res.size(); ++i){
			res[i] = a[i] + b[i];
		}
		return res;
	}

	template<typename S, typename V>
	V mul(const S& a, const V& b) {
		V res = b;
		for(size_t i=0; i<res.size(); ++i){
			res[i] = a * b[i];
		}
		return res;
	}

	template<>
	glm::vec2 add(const glm::vec2& a, const glm::vec2& b) {
		return a + b;
	}

	template<>
	glm::vec2 mul(const float& a, const glm::vec2& b) {
		return a * b;
	}

	template<>
	glm::vec3 add(const glm::vec3& a, const glm::vec3& b) {
		return a + b;
	}

	template<>
	glm::vec3 mul(const float& a, const glm::vec3& b) {
		return a * b;
	}

	template<>
	std::complex<float> add(const std::complex<float>& a, const std::complex<float>& b) {
		return a + b;
	}

	template<>
	std::complex<float> mul(const std::complex<float>& a, const std::complex<float>& b) {
		return a * b;
	}
}

/// Functions for integrion
namespace integrate {
	// Linear step function for N-dimensions
	template<typename S, typename V>
	auto linStep(const V& x0, S dt, const V& dx) {
		return math::add(x0, math::mul(dt, dx));
	}

	// Rotational step function for 2D
	template<typename S, typename V>
	auto rotStep(const V& x0, S dt, S dw) {
		// Make complex r from x0
		auto r = std::complex<float>(x0[0], x0[1]);
		// Compute delta omega for this time step
		dw = dw*dt;
		// Make complex rotation from delta omega
		auto c = std::complex<float>(std::cos(dw), std::sin(dw));
		// Rotate point
		auto x1 = c * r * c;
		return V{x1.real(),x1.imag()};
	}

	// Rotational step function for 3D
	template<typename S, typename V>
	auto rotStep(const V& x0, S dt, const V& dw) {
		// Make complex r from x0
		auto r = x0;
		// Compute delta omega for this time step
		S angle = glm::length(dw)*dt;
		// Make complex rotation from delta omega
		auto q = glm::angleAxis(angle,glm::normalize(dw));
		// Rotate point
		return q * r * glm::conjugate(q);
	}

	template<typename S, typename V, typename DeriveFunc>
	auto eulerl(const V& x0, S dt, DeriveFunc derive) {
		// Kysy paljonko f'(0) on
		auto dx = derive(x0, 0);
		// Laske f(0)+x*f'(x)
		return linStep(x0, dt, dx);
	}

	template<typename S, typename V, typename DeriveFunc>
	auto eulerr(const V& x0, S dt, DeriveFunc derive) {
		// Kysy paljonko f'(0) on
		auto dx = derive(x0, 0);
		// Laske f(0)+x*f'(x)
		return rotStep(x0, dt, dx);
	}

	template<typename S, typename V, typename DeriveFunc>
	auto midPoint(const V& x0, S dt, DeriveFunc derive) {
		// Tutki Eulerin menetelmän avulla paljonko on f(x/2)
		// Kysy paljonko on f'(0)
		auto dx0 = derive(x0, 0);
		// laske f(0)+(x/2)*f'(0)
		auto dx1 = step(x0, dt/S(2), dx0);
		// Nyt voimme soveltaa keskipistemenetelmää
		// Kysy paljonko on f'(x/2), kun f(x/2) tiedetään
		dx0 = derive(dx1, dt/S(2));
		// Laske paljonko on f(0)+x*f'(x/2)
		return step(x0, dt, dx0);
	}

	template<typename S, typename V, typename DeriveFunc>
	auto rungeKutta(const V& x0, S dt, DeriveFunc derive) {
		auto k1 = math::mul(dt, derive(x0, 0));
		auto t2 = step(x0, S(0.5), k1);
		auto k2 = math::mul(dt, derive(t2, S(0.5) * dt));
		auto t3 = step(x0, S(0.5), k2);
		auto k3 = math::mul(dt, derive(t3, S(0.5) * dt));
		auto t4 = math::add(x0, k3);
		auto k4 = math::mul(dt, derive(t4, dt));
		return math::add(math::add(math::add(math::add(x0,
				math::mul(S(1.0/6.0), k1)),
				math::mul(S(1.0/3.0), k2)),
				math::mul(S(1.0/3.0), k3)),
				math::mul(S(1.0/6.0), k4));
	}
}


