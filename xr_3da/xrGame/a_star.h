////////////////////////////////////////////////////////////////////////////
//	Module 		: a_star.h
//	Created 	: 21.03.2002
//  Modified 	: 02.03.2004
//	Author		: Dmitriy Iassenev
//	Description : Implementation of the A* (a-star) algorithm
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "vertex_path.h"
#include "data_storage_constructor.h"
#include "dijkstra.h"

namespace AStar
{
	template <typename _dist_type, template <typename> class T1>
	struct AStar_Vertex
	{
		template <typename T2>
		struct _ai_vertex : public T1<T2>
		{
			typedef _dist_type _dist_type;

			_dist_type _g;
			_dist_type _h;

			IC _dist_type& g() { return (_g); }

			IC _dist_type& h() { return (_h); }
		};
	};

	template <typename _dist_type, template <typename> class T1, typename T2>
	using _a_vertex = typename AStar_Vertex<_dist_type, T1>::_ai_vertex<T2>;

	template <typename _dist_type, typename _priority_queue, typename _vertex_manager, typename _vertex_allocator,
			  bool euclidian_heuristics = true, typename _data_storage_base = CVertexPath<euclidian_heuristics>,
			  template <typename _T> class _vertex = CEmptyClassTemplate,
			  template <typename _1, typename _2> class _builder_allocator_constructor = CBuilderAllocatorConstructor,
			  template <typename _1, typename _2, typename _3, template <typename _1, typename _2> class _4>
			  class _manager_builder_allocator_constructor = CManagerBuilderAllocatorConstructor,
			  template <typename _algorithm, typename _manager, typename _builder, typename _allocator,
						template <typename _T> class _vertex,
						template <typename _1, typename _2> class _builder_allocator_constructor =
							CBuilderAllocatorConstructor,
						template <typename _1, typename _2, typename _3, template <typename _1, typename _2> class _4>
						class _manager_builder_allocator_constructor = CManagerBuilderAllocatorConstructor>
			  class _data_storage_constructor = CDataStorageConstructor,
			  typename _iteration_type = u32>
	struct smart_inherated
	{
		template <typename _T>
		using AStarVertexTemplate = _a_vertex<_dist_type, _vertex, _T>;

		using inheratable =
			CDijkstra<_dist_type, _priority_queue, _vertex_manager, _vertex_allocator, euclidian_heuristics,
					  _data_storage_base, AStarVertexTemplate, _builder_allocator_constructor,
					  _manager_builder_allocator_constructor, _data_storage_constructor, _iteration_type>;
	};

} // namespace AStar

template <typename _dist_type, typename _priority_queue, typename _vertex_manager, typename _vertex_allocator,
		  bool euclidian_heuristics = true, typename _data_storage_base = CVertexPath<euclidian_heuristics>,
		  template <typename _T> class _vertex = CEmptyClassTemplate,
		  template <typename, typename> class _builder_allocator_constructor = CBuilderAllocatorConstructor,
		  template <typename, typename, typename, template <typename, typename> class>
		  class _manager_builder_allocator_constructor = CManagerBuilderAllocatorConstructor,
		  template <typename _algorithm, typename _manager, typename _builder, typename _allocator,
					template <typename _T> class _vertex,
					template <typename, typename> class _builder_allocator_constructor = CBuilderAllocatorConstructor,
					template <typename, typename, typename, template <typename, typename> class>
					class _manager_builder_allocator_constructor = CManagerBuilderAllocatorConstructor>
		  class _data_storage_constructor = CDataStorageConstructor,
		  typename _iteration_type = u32>
class CAStar : public AStar::smart_inherated<_dist_type, _priority_queue, _vertex_manager, _vertex_allocator,
											 euclidian_heuristics, _data_storage_base, _vertex,
											 _builder_allocator_constructor, _manager_builder_allocator_constructor,
											 _data_storage_constructor, _iteration_type>::inheratable
{
protected:
	typedef typename AStar::smart_inherated<_dist_type, _priority_queue, _vertex_manager, _vertex_allocator,
											euclidian_heuristics, _data_storage_base, _vertex,
											_builder_allocator_constructor, _manager_builder_allocator_constructor,
											_data_storage_constructor, _iteration_type>::inheratable inherited;

	typedef typename inherited::CDataStorage::CGraphVertex CGraphVertex;
	typedef typename CGraphVertex::_dist_type _dist_type;
	typedef typename CGraphVertex::_index_type _index_type;

protected:
	template <typename _PathManager>
	IC void initialize(_PathManager& path_manager);
	template <typename _PathManager>
	IC bool step(_PathManager& path_manager);

public:
	IC CAStar(const u32 max_vertex_count);
	virtual ~CAStar();
	template <typename _PathManager>
	IC bool find(_PathManager& path_manager);
};

#include "a_star_inline.h"
