#include "AvroTypes.h"

#include <string.h>

// #include <log4cplus/logger.h>
// #include <log4cplus/loggingmacros.h>


namespace gpudb {

avro_t get_response(avro_t request)
{
  switch(request)
  {
    case register_type_request_avro:            return register_type_response_avro;
    // case register_type_with_annotations_request_avro: return register_type_with_annotations_response_avro;
    case new_set_request_avro:                  return new_set_response_avro;
    case add_object_request_avro:               return add_object_response_avro;
    case get_set_request_avro:                  return get_set_response_avro;
    // case get_set_internal_request_avro:         return get_set_response_avro;
    // case get_set_count_request_avro:            return get_set_count_response_avro;
    // case make_bloom_request_avro:               return make_bloom_response_avro;
    // case get_at_index_request_avro:             return get_at_index_response_avro;
    case bulk_add_request_avro:                 return bulk_add_response_avro;
    // case bulk_add_internal_request_avro:        return bulk_add_response_avro;
    // case set_info_request_avro:                 return set_info_response_avro;
    // case exit_request_avro:                     return exit_response_avro;
    // case get_set_bloom_request_avro:            return get_set_bloom_response_avro;
    case clear_request_avro:                    return clear_response_avro;
    // case copy_set_request_avro:                 return copy_set_response_avro;
    // case stats_request_avro:                    return stats_response_avro;
    // case get_orphans_request_avro:              return get_orphans_response_avro;

  //   case delete_object_request_avro:            return delete_object_response_avro;
    case bounding_box_request_avro:             return bounding_box_response_avro;
  //   case join_request_avro:                     return join_response_avro;
  //   case filter_by_list_request_avro:           return filter_by_list_response_avro;
  //   case filter_by_nai_request_avro:            return filter_by_nai_response_avro;
  //   case filter_by_radius_request_avro:         return filter_by_radius_response_avro;
  //   case filter_by_bounds_request_avro:         return filter_by_bounds_response_avro;
  //   case filter_then_histogram_request_avro:    return filter_then_histogram_response_avro;
  //   case max_min_request_avro:                  return max_min_response_avro;
  // case statistics_request_avro:               return statistics_internal_response_avro;
  //   case select_request_avro:                   return select_response_avro;
  //   case histogram_request_avro:                return histogram_response_avro;
  //   case store_group_by_request_avro:           return store_group_by_response_avro;
  //   case group_by_request_avro:                 return group_by_response_avro;
  //   case group_by_value_request_avro:           return group_by_value_response_avro;
  //   case subset_match_request_avro:             return subset_match_response_avro;
    // case clear_trigger_request_avro:            return clear_trigger_response_avro;
    // case register_trigger_range_request_avro:   return register_trigger_range_response_avro;
    // case register_trigger_nai_request_avro:     return register_trigger_nai_response_avro;
    // case get_trigger_info_request_avro:         return get_trigger_info_response_avro;
    // case update_set_ttl_request_avro:           return update_set_ttl_response_avro;
    // case authenticate_users_request_avro:       return authenticate_users_response_avro;
    // case get_sorted_sets_request_avro:          return get_sorted_sets_response_avro;
    // case get_full_set_request_avro:             return get_full_set_response_avro;
    // case get_tom_set_stats_request_avro:        return get_tom_set_stats_response_avro;
    // case register_type_transform_request_avro:  return register_type_transform_response_avro;
    // case get_objects_request_avro:              return get_objects_response_avro;
    // case get_type_info_request_avro:            return get_type_info_response_avro;
    // case get_sets_by_type_info_request_avro:    return get_sets_by_type_info_response_avro;

  //   case update_trigger_request_avro:           return update_trigger_response_avro;
  //   case spatial_query_request_avro:            return spatial_query_response_avro;

  // case spatial_set_query_request_avro:        return spatial_set_query_internal_response_avro;

    // case filter_by_string_request_avro:         return filter_by_string_response_avro;
    // case filter_by_value_request_avro:          return filter_by_value_response_avro;
    // case get_tracks_request_avro:               return get_tracks_response_avro;
    // case get_tracks2_request_avro:              return get_tracks2_response_avro;
    // case get_tom_tracks_request_avro:           return get_tracks2_response_avro;
    // case get_sorted_set_request_avro:           return get_sorted_set_response_avro;
    // case get_sorted_set_internal_request_avro:  return get_sorted_set_internal_response_avro;
    // case random_request_avro:                   return random_response_avro;
    // case clear_auth_cache_request_avro:         return clear_auth_cache_response_avro;
    case status_request_avro:                   return status_response_avro;
    // case get_set_sizes_request_avro:            return get_set_sizes_response_avro;
    // case update_object_request_avro:            return update_object_response_avro;
    // case unique_request_avro:                   return unique_response_avro;
    // case generate_video_request_avro:           return generate_video_response_avro;

  // case generate_heatmap_video_request_avro:   return generate_heatmap_video_internal_response_avro;

    // case logger_request_avro:                   return logger_response_avro;
    // case server_status_request_avro:            return server_status_response_avro;
    // case convex_hull_request_avro:              return convex_hull_response_avro;
    // case filter_by_set_request_avro:            return filter_by_set_response_avro;
    // case populate_full_tracks_request_avro:     return populate_full_tracks_response_avro;
    // case register_parent_set_request_avro:      return register_parent_set_response_avro;
    // case add_symbol_request_avro:               return add_symbol_response_avro;
    // case update_set_metadata_request_avro:      return update_set_metadata_response_avro;
    // case get_set_metadata_request_avro:         return get_set_metadata_response_avro;
    // case update_set_properties_request_avro:    return update_set_properties_response_avro;
    // case get_set_properties_request_avro:       return get_set_properties_response_avro;

    // case select_delete_request_avro:            return select_delete_response_avro;
    // case select_update_request_avro:            return select_update_response_avro;
    // case filter_by_track_request_avro:          return filter_by_track_response_avro;
    // case filter_by_track_values_request_avro:   return filter_by_track_values_response_avro;
    // case k_means_internal_request_avro:         return k_means_internal_response_avro;
    // case k_means_request_avro:                  return k_means_response_avro;
    // case bulk_update_request_avro:              return bulk_update_response_avro;

    default:
    {
      // log4cplus::Logger logger = log4cplus::Logger::getInstance("default");
      // LOG4CPLUS_WARN(logger, "Unknown avro_t request type: " << request << " in get_response(avro_t)");
      return invalid_response_type_avro;
    }
  }
}

std::string get_endpoint(avro_t type)
{
  switch (type)
  {
     // case bulk_add_request_avro:                return "/bulkadd";
     // case bulk_add_internal_request_avro:       return "/bulkadd";
     // case bounding_box_request_avro:            return "/boundingbox";
     // case join_request_avro:                    return "/join";
     // case filter_by_list_request_avro:          return "/filterbylist";
     // case filter_by_nai_request_avro:           return "/filterbynai";
     // case filter_by_radius_request_avro:        return "/filterbyradius";
     // case filter_by_bounds_request_avro:        return "/filterbybounds";
     // case filter_by_string_request_avro:        return "/filterbystring";
     // case filter_then_histogram_request_avro:   return "/filterthenhistogram";
     // case max_min_request_avro:                 return "/maxmin";
     // case statistics_request_avro:              return "/statistics";
     // case select_request_avro:                  return "/select";
     // case histogram_request_avro:               return "/histogram";
     // case store_group_by_request_avro:          return "/storegroupby";
     // case group_by_request_avro:                return "/groupby";
     // case group_by_value_request_avro:          return "/groupbyvalue";
     // case subset_match_request_avro:            return "/subsetmatch";
     // case plot2d_multiple_2_request_avro:       return "/plot2dmultiple2";
     // case plot2d_multiple_cb_request_avro:      return "/plot2dmultiplecb";
     // case plot2d_heatmap_request_avro:          return "/plot2heatmap";
     // case plot2d_heatmap_cb_request_avro:       return "/plot2heatmapcb";
     // case join_incremental_request_avro:        return "/joinincremental";
     // case clear_request_avro:                   return "/clear";
     // case get_objects_request_avro:             return "/getobjects";
     // case delete_object_request_avro:           return "/deleteobject";
     // case get_tracks_request_avro:              return "/gettracks";
     // case get_tracks2_request_avro:             return "/gettracks2";
     // case get_sorted_set_request_avro:          return "/getsortedset";
     // case random_request_avro:                  return "/random";
     // case clear_auth_cache_request_avro:        return "/clearauthcache";
     case status_request_avro:                  return "/status";
     // case get_set_sizes_request_avro:           return "/getsetsizes";
     // case update_object_request_avro:           return "/updateobject";
     // case unique_request_avro:                  return "/unique";
     // case generate_video_request_avro:          return "/generatevideo";
     // case generate_heatmap_video_request_avro:  return "/generateheatmapvideo";
     // case logger_request_avro:                  return "/logger";
     // case server_status_request_avro:           return "/serverstatus";
     // case convex_hull_request_avro:             return "/convexhull";
     // case filter_by_set_request_avro:           return "/filterbyset";
     // case populate_full_tracks_request_avro:    return "/populatefulltracks";
     // case register_parent_set_request_avro:     return "/registerparentset";
     // case add_symbol_request_avro:              return "/addsymbol";
     // case update_set_metadata_request_avro:     return "/updatesetmetadata";
     // case get_set_metadata_request_avro:        return "/getsetmetadata";
     // case update_set_properties_request_avro:   return "/updatesetproperties";
     // case get_set_properties_request_avro:      return "/getsetproperties";
     // case get_tom_set_stats_request_avro:       return "/gettomsetstats";
     // case select_delete_request_avro:           return "/selectdelete";
     // case select_update_request_avro:           return "/selectupdate";
     // case filter_by_track_request_avro:         return "/filterbytrack";
     // case filter_by_track_values_request_avro:  return "/filterbytrackvalues";
     // case k_means_request_avro:                 return "/kmeans";
     // case k_means_internal_request_avro:        return "/kmeansinternal";
     // case bulk_update_request_avro:             return "/bulkupdate";
     default:                                   return "unknown";
  }
}


// Really do not like these functions; need to just rewrite this avro enum/class/endpoint
/// Get the avro_t type corresponding to the given endpoint
/// E.G. "/boundingbox" returns avro_t::bounding_box_request_avro
avro_t get_avro_type(std::string endpoint)
{
    if ( endpoint == "/add" )
        return add_object_request_avro;
    if ( endpoint == "/bulkadd" )
        return bulk_add_request_avro;
    if ( endpoint == "/boundingbox" )
        return bounding_box_request_avro;
    if ( endpoint == "/clear" )
        return clear_request_avro;
    if ( endpoint == "/getset" )
        return get_set_request_avro;
    if ( endpoint == "/newset" )
        return new_set_request_avro;
    if ( endpoint == "/registerparentset" )
        return register_parent_set_request_avro;
    if ( endpoint == "/registertype" )
        return register_type_request_avro;
    if ( endpoint == "/status" )
        return status_request_avro;
    // Unsupported request Avro type
    return invalid_response_type_avro;
    // case filter_by_list_request_avro:          return "/filterbylist";
    // case filter_by_nai_request_avro:           return "/filterbynai";
    // case filter_by_radius_request_avro:        return "/filterbyradius";
    // case filter_by_bounds_request_avro:        return "/filterbybounds";
    // case filter_by_string_request_avro:        return "/filterbystring";
    // case filter_then_histogram_request_avro:   return "/filterthenhistogram";
    // case max_min_request_avro:                 return "/maxmin";
    // case statistics_request_avro:              return "/statistics";
    // case select_request_avro:                  return "/select";
    // case histogram_request_avro:               return "/histogram";
    // case store_group_by_request_avro:          return "/storegroupby";
    // case group_by_request_avro:                return "/groupby";
    // case group_by_value_request_avro:          return "/groupbyvalue";
    // case subset_match_request_avro:            return "/subsetmatch";
    // case get_objects_request_avro:             return "/getobjects";
    // case delete_object_request_avro:           return "/deleteobject";
    // case get_tracks_request_avro:              return "/gettracks";
    // case get_tracks2_request_avro:             return "/gettracks2";
    // case get_sorted_set_request_avro:          return "/getsortedset";
    // case random_request_avro:                  return "/random";
    // case clear_auth_cache_request_avro:        return "/clearauthcache";
    // case get_set_sizes_request_avro:           return "/getsetsizes";
    // case update_object_request_avro:           return "/updateobject";
    // case unique_request_avro:                  return "/unique";
    // case generate_video_request_avro:          return "/generatevideo";
    // case generate_heatmap_video_request_avro:  return "/generateheatmapvideo";
    // case logger_request_avro:                  return "/logger";
    // case server_status_request_avro:           return "/serverstatus";
    // case convex_hull_request_avro:             return "/convexhull";
    // case filter_by_set_request_avro:           return "/filterbyset";
    // case populate_full_tracks_request_avro:    return "/populatefulltracks";
    // case add_symbol_request_avro:              return "/addsymbol";
    // case update_set_metadata_request_avro:     return "/updatesetmetadata";
    // case get_set_metadata_request_avro:        return "/getsetmetadata";
    // case update_set_properties_request_avro:   return "/updatesetproperties";
    // case get_set_properties_request_avro:      return "/getsetproperties";
    // case get_tom_set_stats_request_avro:       return "/gettomsetstats";
    // case select_delete_request_avro:           return "/selectdelete";
    // case select_update_request_avro:           return "/selectupdate";
    // case filter_by_track_request_avro:         return "/filterbytrack";
    // case filter_by_track_values_request_avro:  return "/filterbytrackvalues";
    // case k_means_request_avro:                 return "/kmeans";
    // case k_means_internal_request_avro:        return "/kmeansinternal";
    // case bulk_update_request_avro:             return "/bulkupdate";
} // end get_avrotype


} // namespace gpudb
