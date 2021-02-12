#include "Outline.hpp"

namespace pf {
	Outline Outline::withCapacity(std::size_t cap) {
		Outline ret;
		ret.contours.reserve(cap);
		return ret;
	}
	Outline Outline::fromRect(const RectF& rect) {
		Outline ret;
		ret.push(Contour::fromRect(rect));
		return ret;
	}
	Outline Outline::fromRectRounded(const RectF& rect, const glm::vec2& radius) {
		Outline ret;
		ret.push(Contour::fromRectRounded(rect, radius));
		return ret;
	}

	Outline::Outline()
	{}

	void Outline::clear() {
		contours.clear();
		bounds = RectF{};
	}

	void Outline::closeAllContours() noexcept {
		for (Contour & contour : contours) {
			contour.close();
		}
	}

	void Outline::recalculateBounds() noexcept {
		if (empty()) {
			bounds = RectF{};
		}
		else {
			bounds = contours.front().bounds;
			for (auto it = contours.begin() + 1, last = contours.end(); it != last; ++it) {
				bounds = bounds.merge(it->bounds);
			}
		}
	}

	// number of contours in the outline.
	std::size_t Outline::size() const noexcept {
		return contours.size();
	}
	bool Outline::empty() const noexcept {
		return contours.empty();
	}

	void Outline::push(const Outline& other) {
		if (other.empty()) {
			return;
		}

		for (const Contour& con : other) {
			contours.push_back(con);
		}

		if (empty()) {
			bounds = other.bounds;
		}
		else {
			bounds = bounds.merge(other.bounds);
		}
	}
	void Outline::push(const Contour& contour) {
		if (contour.empty()) {
			return;
		}

		contours.push_back(contour);

		if (contours.empty()) {
			bounds = contour.bounds;
		}
		else {
			bounds = bounds.merge(contour.bounds);
		}
	}
	void Outline::push(Outline&& other) {
		if (other.empty()) {
			return;
		}

		if (empty()) {
			bounds = other.bounds;
		}
		else {
			bounds = bounds.merge(other.bounds);
		}
		other.bounds = RectF{};

		for(Contour & contour : other) {
			contours.push_back(std::move(contour));
		}
		other.contours.clear();
	}
	void Outline::push(Contour&& contour) {
		if (contour.empty()) {
			return;
		}

		if (contours.empty()) {
			bounds = contour.bounds;
		}
		else {
			bounds = bounds.merge(contour.bounds);
		}

		contours.push_back(std::move(contour));
	}
	Contour Outline::pop() {
		assert(!empty());
		Contour ret = std::move(contours.back());
		contours.pop_back();

		recalculateBounds();
		return ret;
	}

	void Outline::transform(const Transform2F& form) {
		if (form.isIdentity()) {
			return;
		}

		for (Contour& contour : contours) {
			contour.transform(form);
		}
		recalculateBounds();
	}
	Outline Outline::transformed(const Transform2F& form) const {
		Outline copy = *this;
		copy.transform(form);
		return copy;
	}

	void Outline::dilate(float amount) {
		dilate({ amount, amount });
	}
	void Outline::dilate(const glm::vec2& amount) {

	}

	using iterator = Outline::iterator;
	using const_iterator = Outline::const_iterator;
	using reverse_iterator = Outline::reverse_iterator;
	using const_reverse_iterator = Outline::const_reverse_iterator;

	iterator Outline::begin() noexcept {
		return contours.begin();
	}
	iterator Outline::end() noexcept {
		return contours.end();
	}

	const_iterator Outline::begin() const noexcept {
		return contours.begin();
	}
	const_iterator Outline::end() const noexcept {
		return contours.end();
	}

	const_iterator Outline::cbegin() const noexcept {
		return contours.cbegin();
	}
	const_iterator Outline::cend() const noexcept {
		return contours.cend();
	}

	reverse_iterator Outline::rbegin() noexcept {
		return contours.rbegin();
	}
	reverse_iterator Outline::rend() noexcept {
		return contours.rend();
	}

	const_reverse_iterator Outline::rbegin() const noexcept {
		return contours.rbegin();
	}
	const_reverse_iterator Outline::rend() const noexcept {
		return contours.rend();
	}

	const_reverse_iterator Outline::crbegin() const noexcept {
		return contours.crbegin();
	}
	const_reverse_iterator Outline::crend() const noexcept {
		return contours.crend();
	}
};