////////////////////////////////////////////////////////////////////////////
//	Module 		: data_storage_bucket_list.h
//	Created 	: 21.03.2002
//  Modified 	: 26.02.2004
//	Author		: Dmitriy Iassenev
//	Description : Bucket list data storage
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "data_storage_double_linked_list.h"

template <typename _path_id_type, typename _bucket_id_type, u32 bucket_count, bool clear_buckets>
struct CDataStorageBucketList
{
	template <template <typename> class TBase>
	struct BucketList
	{
		template <typename TInternal>
		struct _bucket_vertex : public TBase<TInternal>
		{
			_path_id_type m_path_id;
			_bucket_id_type m_bucket_id;
		};
	};

	template <template <typename> class T1, typename T2>
	using _bucket_vertex = typename BucketList<T1>::_bucket_vertex<T2>;

	template <typename _data_storage, template <typename> class _vertex = CEmptyClassTemplate>
	struct smart_inherated
	{
		template <typename _T>
		using BucketVertexTemplate = _bucket_vertex<_vertex, _T>;

		using inheratable = CDataStorageDoubleLinkedList<false>::CDataStorage<_data_storage, BucketVertexTemplate>;
	};

	template <typename _data_storage, template <typename> class _vertex = CEmptyClassTemplate>
	class CDataStorage : public smart_inherated<_data_storage, _vertex>::inheratable
	{
	public:
		typedef typename smart_inherated<_data_storage, _vertex>::inheratable inherited;
		typedef typename inherited::inherited_base inherited_base;
		typedef typename inherited::CGraphVertex CGraphVertex;
		typedef typename CGraphVertex::_dist_type _dist_type;
		typedef typename CGraphVertex::_index_type _index_type;

	protected:
		_dist_type m_min_bucket_value;
		_dist_type m_max_bucket_value;
		CGraphVertex* m_buckets[bucket_count];
		u32 m_min_bucket_id;

	public:
		IC CDataStorage(const u32 vertex_count);
		virtual ~CDataStorage();
		IC void init();
		IC void add_best_closed();
		IC bool is_opened_empty();
		IC u32 compute_bucket_id(CGraphVertex& vertex) const;
		IC void verify_buckets() const;
		IC void add_to_bucket(CGraphVertex& vertex, u32 bucket_id);
		IC void add_opened(CGraphVertex& vertex);
		IC void decrease_opened(CGraphVertex& vertex, const _dist_type value);
		IC void remove_best_opened();
		IC CGraphVertex& get_best();
		IC void set_min_bucket_value(const _dist_type min_bucket_value);
		IC void set_max_bucket_value(const _dist_type max_bucket_value);
	};
};

#include "data_storage_bucket_list_inline.h"
