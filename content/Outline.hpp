#pragma once
#include <cinttypes>
#include <vector>
#include <glm/vec2.hpp>
#include <optional>
#include <../geometry/RectF.hpp>
#include <../geometry/Transform2F.hpp>

namespace pf {
	enum class PointFlags : uint8_t {
		None = 0,
		Control0 = 1,
		Control1 = 2,
		All = 3
	};

	struct Contour {
		struct Point {
			glm::vec2 point;
			PointFlags flags;
		};

		using container_t = std::vector<Point>;

		using iterator = container_t::iterator;
		using const_iterator = container_t::const_iterator;
		using reverse_iterator = container_t::reverse_iterator;
		using const_reverse_iterator = container_t::const_reverse_iterator;

		static Contour withCapacity(std::size_t cap);
		static Contour fromRect(const RectF & rect);
		static Contour fromRoundedRect(const RectF& rect, const glm::vec2& radius);


		Contour();

		void clear() noexcept;
		bool isEmpty() const noexcept;
		bool isClosed() const noexcept;
		std::size_t size() const noexcept;

		void close();
		void updateBounds();

		void pushPoint(const glm::vec2& p, PointFlags pflags, bool updateBounds);
		//void pushSegment();
		void pushEllipse(const Transform2F& form);

		void pushQuadratic(const glm::vec2 & p0, const glm::vec2 & p1);
		void pushCubic(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2 & p2);

		iterator begin() noexcept;
		iterator end() noexcept;

		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;

		reverse_iterator rbegin() noexcept;
		reverse_iterator rend() noexcept;

		const_reverse_iterator rbegin() const noexcept;
		const_reverse_iterator rend() const noexcept;

		std::vector<Point> points;
		RectF bounds;
		bool closed;
	};

	struct Outline {
		static Outline withCapacity(std::size_t cap);

		Outline();


		std::vector<Contour> contours;
		RectF bounds;
	};


};

pf::PointFlags operator|(pf::PointFlags lh, pf::PointFlags rh) noexcept;
pf::PointFlags operator^(pf::PointFlags lh, pf::PointFlags rh) noexcept;
pf::PointFlags operator&(pf::PointFlags lh, pf::PointFlags rh) noexcept;
pf::PointFlags operator~(pf::PointFlags lh) noexcept;