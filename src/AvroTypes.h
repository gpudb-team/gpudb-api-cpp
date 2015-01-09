#ifndef __AVRO_TYPES_H__
#define __AVRO_TYPES_H__

#include <string>


namespace gpudb {

  
  // NOTE: We might need to expand these, i.e. branch out into requests and responses
  enum avro_t
  {
      invalid_request_type_avro = -1,       invalid_response_type_avro = -1,

      register_type_request_avro,           register_type_response_avro,
      register_type_with_annotations_request_avro, register_type_with_annotations_response_avro,
      new_set_request_avro,                 new_set_response_avro,
      add_object_request_avro,              add_object_response_avro,
      get_set_request_avro,                 get_set_internal_request_avro, get_set_response_avro,
      get_set_objects_request_avro,         get_set_objects_response_avro,
      get_set_count_request_avro,           get_set_count_response_avro,
      bounding_box_request_avro,            bounding_box_response_avro,
      join_request_avro,                    join_response_avro,
      group_by_request_avro,                group_by_response_avro,
      group_by_value_request_avro,          group_by_value_response_avro,
      exit_request_avro,                    exit_response_avro,
      make_bloom_request_avro,              make_bloom_response_avro,
      get_set_bloom_request_avro,           get_set_bloom_response_avro,
      store_group_by_request_avro,          store_group_by_response_avro,
      bulk_add_request_avro,                bulk_add_internal_request_avro, bulk_add_response_avro,
      histogram_request_avro,               histogram_response_avro,
      max_min_request_avro,                 max_min_response_avro,
      statistics_request_avro,              statistics_response_avro, statistics_internal_response_avro,
      filter_by_bounds_request_avro,        filter_by_bounds_response_avro,
      filter_by_list_request_avro,          filter_by_list_response_avro,
      filter_by_nai_request_avro,           filter_by_nai_response_avro,
      clear_request_avro,                   clear_response_avro,
      copy_set_request_avro,                copy_set_response_avro,
      stats_request_avro,                   stats_response_avro,
      select_request_avro,                  select_response_avro,
      join_setup_request_avro,              join_setup_response_avro,
      subset_match_request_avro,            subset_match_response_avro,
      join_incremental_request_avro,        join_incremental_response_avro,
      get_at_index_request_avro,            get_at_index_response_avro,
      set_info_request_avro,                set_info_response_avro,
      filter_by_radius_request_avro,        filter_by_radius_response_avro,
      filter_then_histogram_request_avro,   filter_then_histogram_response_avro,
      plot2d_multiple_2_request_avro,       plot2d_multiple_2_response_avro,
      plot2d_multiple_cb_request_avro,      plot2d_multiple_cb_response_avro,
      plot2d_heatmap_request_avro,          plot2d_heatmap_response_avro, plot2d_heatmap_internal_response_avro,
      plot2d_heatmap_cb_request_avro,       plot2d_heatmap_cb_response_avro, plot2d_heatmap_cb_internal_response_avro,
      clear_trigger_request_avro,           clear_trigger_response_avro,
      register_trigger_range_request_avro,  register_trigger_range_response_avro,
      register_trigger_nai_request_avro,    register_trigger_nai_response_avro,
      get_trigger_info_request_avro,        get_trigger_info_response_avro,
      update_set_ttl_request_avro,          update_set_ttl_response_avro,
      authenticate_users_request_avro,      authenticate_users_response_avro,
      get_sorted_sets_request_avro,         get_sorted_sets_response_avro,
      get_sorted_set_request_avro,          get_sorted_set_response_avro,
      get_sorted_set_internal_request_avro, get_sorted_set_internal_response_avro,
      predicate_join_request_avro,          predicate_join_response_avro,
      get_full_set_request_avro,            get_full_set_response_avro,
      get_tom_set_stats_request_avro,       get_tom_set_stats_response_avro,
      road_intersection_request_avro,       road_intersection_response_avro, road_intersection_internal_response_avro,
      register_type_transform_request_avro, register_type_transform_response_avro,
      get_objects_request_avro,             get_objects_response_avro,
      get_type_info_request_avro,           get_type_info_response_avro,
      get_sets_by_type_info_request_avro,   get_sets_by_type_info_response_avro,
      merge_sets_request_avro,              merge_sets_response_avro,
      get_orphans_request_avro,             get_orphans_response_avro,
      update_trigger_request_avro,          update_trigger_response_avro,
      spatial_query_request_avro,           spatial_query_response_avro,
      spatial_set_query_request_avro,       spatial_set_query_response_avro, spatial_set_query_internal_response_avro,
      delete_object_request_avro,           delete_object_response_avro,
      filter_by_string_request_avro,        filter_by_string_response_avro,
      filter_by_value_request_avro,         filter_by_value_response_avro,
      get_tracks_request_avro,              get_tracks_response_avro,
      get_tracks2_request_avro,             get_tracks2_response_avro,  get_tom_tracks_request_avro,
      random_request_avro,                  random_response_avro,
      clear_auth_cache_request_avro,        clear_auth_cache_response_avro,
      status_request_avro,                  status_response_avro,
      get_set_sizes_request_avro,           get_set_sizes_response_avro,
      update_object_request_avro,           update_object_response_avro,
      unique_request_avro,                  unique_response_avro,
      generate_video_request_avro,          generate_video_response_avro,
      generate_heatmap_video_request_avro,  generate_heatmap_video_response_avro, generate_heatmap_video_internal_response_avro,
      logger_request_avro,                  logger_response_avro,
      server_status_request_avro,           server_status_response_avro,
      convex_hull_request_avro,             convex_hull_response_avro,
      filter_by_set_request_avro,           filter_by_set_response_avro,
      populate_full_tracks_request_avro,    populate_full_tracks_response_avro,
      register_parent_set_request_avro,     register_parent_set_response_avro,
      add_symbol_request_avro,              add_symbol_response_avro,
      update_set_metadata_request_avro,     update_set_metadata_response_avro,
      get_set_metadata_request_avro,        get_set_metadata_response_avro,
      update_set_properties_request_avro,   update_set_properties_response_avro,
      get_set_properties_request_avro,      get_set_properties_response_avro,
      add_string_hash_request_avro,         add_string_hash_response_avro,
      bulk_add_data_request_avro,           bulk_add_data_response_avro,
      get_string_hash_request_avro,         get_string_hash_response_avro,
      get_user_auths_request_avro,          get_user_auths_response_avro,
      select_delete_request_avro,           select_delete_response_avro,
      select_update_request_avro,           select_update_response_avro,
      filter_by_track_request_avro,         filter_by_track_response_avro,
      filter_by_track_values_request_avro,  filter_by_track_values_response_avro,
      k_means_request_avro,                 k_means_response_avro, k_means_internal_request_avro, k_means_internal_response_avro,
      bulk_update_request_avro,             bulk_update_response_avro,

      actor_list_avro,
      actor_object_avro,
      trigger_notification_avro,

      gpudb_response_avro,
      unknown_uri_avro,

      avro_t_COUNT  ///< The number of valid avro_t types.
  };

  // Don't love this...used in generic calculation task
  /// Returns the avro_t::*_response for the given avro_t::*_request, used for generic tasks.
  /// E.G. avro_t::register_type_request_avro returns avro_t::register_type_response_avro.
  avro_t get_response(avro_t request);

  /// Get the string name of the avro_t enum.
  /// Ex. avro_t::register_type_request_avro return "register_type_request"
  /// Body of function is autogenerated in ../obj_defs/obj_defs.cpp
  std::string get_avro_name(avro_t avro_type);

  // Really do not like these functions; need to just rewrite this avro enum/class/endpoint
  /// Get the Gaia endpoint that handles the avro_t type.
  /// E.G. avro_t::bounding_box_request_avro returns "/boundingbox"
  std::string get_endpoint(avro_t type);

  // Really do not like these functions; need to just rewrite this avro enum/class/endpoint
  /// Get the avro_t type corresponding to the given endpoint
  /// E.G. "/boundingbox" returns avro_t::bounding_box_request_avro
  avro_t get_avro_type(std::string endpoint);

} // namespace gpudb

#endif // __AVRO_TYPES_H__
