#pragma once

#include <polyfem/mesh/Mesh.hpp>
#include <polyfem/Common.hpp>

namespace polyfem
{
	namespace utils
	{
		class Selection
		{
		public:
			typedef std::array<RowVectorNd, 2> BBox;

			Selection(const int id) : id_(id) {}

			virtual ~Selection() {}

			virtual bool inside(const RowVectorNd &p) const = 0;

			virtual int id(const size_t element_id) const { return id_; }

			static std::shared_ptr<Selection> build(
				const json &selection,
				const BBox &mesh_bbox);

		protected:
			Selection() {}

			size_t id_;
		};

		///////////////////////////////////////////////////////////////////////

		class BoxSelection : public Selection
		{
		public:
			BoxSelection(
				const json &selection,
				const BBox &mesh_bbox);

			bool inside(const RowVectorNd &p) const override;

		protected:
			BBox bbox_;
		};

		///////////////////////////////////////////////////////////////////////

		class SphereSelection : public Selection
		{
		public:
			SphereSelection(
				const json &selection,
				const BBox &mesh_bbox);

			bool inside(const RowVectorNd &p) const override;

		protected:
			RowVectorNd center_;
			double radius2_;
		};

		///////////////////////////////////////////////////////////////////////

		class AxisPlaneSelection : public Selection
		{
		public:
			AxisPlaneSelection(
				const json &selection,
				const BBox &mesh_bbox);

			bool inside(const RowVectorNd &p) const override;

		protected:
			int axis_;
			double position_;
		};

		///////////////////////////////////////////////////////////////////////

		class PlaneSelection : public Selection
		{
		public:
			PlaneSelection(
				const json &selection,
				const BBox &mesh_bbox);

			bool inside(const RowVectorNd &p) const override;

		protected:
			RowVectorNd normal_;
			RowVectorNd point_;
		};

		///////////////////////////////////////////////////////////////////////

		class UniformSelection : public Selection
		{
		public:
			UniformSelection(const int id)
				: Selection(id) {}

			bool inside(const RowVectorNd &p) const override { return true; }
		};

		///////////////////////////////////////////////////////////////////////

		class SpecifiedSelection : public Selection
		{
		public:
			SpecifiedSelection(
				const std::vector<int> &ids);

			bool inside(const RowVectorNd &p) const override { return true; }

			int id(const size_t element_id) const override;

		protected:
			SpecifiedSelection() {}

			std::vector<int> ids_;
		};

		///////////////////////////////////////////////////////////////////////

		class FileSelection : public SpecifiedSelection
		{
		public:
			FileSelection(
				const std::string &file_path,
				const int id_offset = 0);
		};
	} // namespace utils
} // namespace polyfem
