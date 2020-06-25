BlinkSrc = $(BkRoot)src/chromium/third_party/blink/renderer
BlinkFlags = -I$(BkRoot)src/blink -I$(BkRoot)src/stub/icu -DBLINKIT_CRAWLER_ONLY $(CrFlags) -include _pc.h
BlinkObjects = duk.o duk_attr.o duk_console.o duk_container_node.o duk_document.o duk_element.o duk_event.o duk_event_listener.o duk_event_target.o duk_exception_state.o duk_html_collection.o duk_location.o duk_named_node_map.o duk_navigator.o duk_node.o duk_node_list.o duk_script_element.o duk_script_object.o duk_window.o prototype_helper.o script_controller.o script_source_code.o script_streamer.o blink_initializer.o css_primitive_value_unit_trie.o css_selector.o css_selector_list.o css_parser.o css_parser_context.o css_parser_selector.o css_parser_token.o css_parser_token_range.o css_parser_token_stream.o css_selector_parser.o css_tokenizer.o css_tokenizer_input_stream.o selector_checker.o selector_query.o attr.o cdata_section.o character_data.o child_list_mutation_scope.o child_node_list.o class_collection.o comment.o container_node.o context_lifecycle_notifier.o context_lifecycle_observer.o decoded_data_document_parser.o document.o document_encoding_data.o document_fragment.o document_init.o document_lifecycle.o document_parser.o document_shutdown_notifier.o document_shutdown_observer.o document_type.o element.o element_data.o element_data_cache.o element_rare_data.o empty_node_list.o add_event_listener_options_resolved.o event.o event_dispatcher.o event_dispatch_forbidden_scope.o event_listener_map.o event_path.o event_target.o node_event_context.o registered_event_listener.o tree_scope_event_context.o window_event_context.o id_target_observer_registry.o live_node_list_base.o live_node_list_registry.o mutation_observer_interest_group.o mutation_record.o named_node_map.o node.o node_child_removal_tracker.o node_lists_node_data.o node_rare_data.o node_traversal.o nth_index_cache.o qualified_name.o range.o scriptable_document_parser.o space_split_string.o synchronous_mutation_notifier.o synchronous_mutation_observer.o tag_collection.o text.o tree_ordered_map.o tree_scope.o tree_scope_adopter.o editing_utilities.o markup_accumulator.o markup_formatter.o serialization.o event_type_names.o execution_context.o web_document_loader_impl.o dom_window.o frame.o frame_lifecycle.o local_dom_window.o local_frame.o location.o navigator.o navigator_id.o navigator_language.o html_collection.o html_document.o html_tag_collection.o atomic_html_token.o compact_html_token.o html_construction_site.o html_document_parser.o html_element_stack.o html_entity_parser.o html_entity_search.o html_formatting_element_list.o html_meta_charset_parser.o html_parser_idioms.o html_parser_options.o html_parser_reentry_permit.o html_preload_scanner.o html_resource_preloader.o html_source_tracker.o html_tokenizer.o html_tree_builder.o html_tree_builder_simulator.o preload_request.o resource_preloader.o text_resource_decoder.o html_element_lookup_trie.o html_entity_table.o html_names.o html_tokenizer_names.o base_fetch_context.o document_loader.o frame_fetch_context.o frame_loader.o frame_loader_state_machine.o frame_load_request.o navigation_scheduler.o script_resource.o text_resource.o scheduled_navigation.o text_resource_decoder_builder.o classic_pending_script.o classic_script.o fetch_client_settings_object_impl.o html_parser_script_runner.o pending_script.o script_element_base.o script_loader.o script_runner.o xlink_names.o xmlns_names.o xml_names.o exception_state.o gc_pool.o script_forbidden_scope.o script_wrappers.o platform.o language.o fetch_context.o fetch_parameters.o raw_resource.o resource.o resource_client.o resource_error.o resource_fetcher.o resource_loader.o resource_request.o resource_response.o source_keyed_cached_metadata_handler.o text_resource_decoder_options.o unique_identifier.o header_field_tokenizer.o http_names.o http_parsers.o content_type.o mime_type_registry.o parsed_content_header_field_parameters.o parsed_content_type.o server_timing_header.o frame_scheduler_impl.o shared_buffer.o segmented_string.o timer.o kurl.o security_policy.o web_task_runner.o ascii_ctype.o decimal.o dtoa.o bignum-dtoa.o bignum.o cached-powers.o diy-fp.o double-conversion.o fast-dtoa.o fixed-dtoa.o strtod.o dynamic_annotations.o hash_table.o atomic_string.o atomic_string_table.o cstring.o string_builder.o string_concatenate.o string_impl.o string_statics.o string_to_number.o string_view.o text_codec.o text_codec_iconv.o text_codec_latin1.o text_codec_replacement.o text_codec_user_defined.o text_codec_user_defined_posix.o text_codec_utf16.o text_codec_utf8.o text_encoding.o text_encoding_registry.o text_position.o unicode_posix.o utf8.o wtf_string.o threading.o time.o wtf.o wtf_thread_data.o

duk.o: $(BlinkSrc)/bindings/core/duk/duk.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
duk_attr.o: $(BlinkSrc)/bindings/core/duk/duk_attr.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
duk_console.o: $(BlinkSrc)/bindings/core/duk/duk_console.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
duk_container_node.o: $(BlinkSrc)/bindings/core/duk/duk_container_node.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
duk_document.o: $(BlinkSrc)/bindings/core/duk/duk_document.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
duk_element.o: $(BlinkSrc)/bindings/core/duk/duk_element.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
duk_event.o: $(BlinkSrc)/bindings/core/duk/duk_event.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
duk_event_listener.o: $(BlinkSrc)/bindings/core/duk/duk_event_listener.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
duk_event_target.o: $(BlinkSrc)/bindings/core/duk/duk_event_target.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
duk_exception_state.o: $(BlinkSrc)/bindings/core/duk/duk_exception_state.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
duk_html_collection.o: $(BlinkSrc)/bindings/core/duk/duk_html_collection.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
duk_location.o: $(BlinkSrc)/bindings/core/duk/duk_location.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
duk_named_node_map.o: $(BlinkSrc)/bindings/core/duk/duk_named_node_map.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
duk_navigator.o: $(BlinkSrc)/bindings/core/duk/duk_navigator.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
duk_node.o: $(BlinkSrc)/bindings/core/duk/duk_node.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
duk_node_list.o: $(BlinkSrc)/bindings/core/duk/duk_node_list.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
duk_script_element.o: $(BlinkSrc)/bindings/core/duk/duk_script_element.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
duk_script_object.o: $(BlinkSrc)/bindings/core/duk/duk_script_object.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
duk_window.o: $(BlinkSrc)/bindings/core/duk/duk_window.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
prototype_helper.o: $(BlinkSrc)/bindings/core/duk/prototype_helper.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
script_controller.o: $(BlinkSrc)/bindings/core/duk/script_controller.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
script_source_code.o: $(BlinkSrc)/bindings/core/duk/script_source_code.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
script_streamer.o: $(BlinkSrc)/bindings/core/duk/script_streamer.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
blink_initializer.o: $(BlinkSrc)/controller/blink_initializer.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
css_primitive_value_unit_trie.o: $(BlinkSrc)/core/css/css_primitive_value_unit_trie.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
css_selector.o: $(BlinkSrc)/core/css/css_selector.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
css_selector_list.o: $(BlinkSrc)/core/css/css_selector_list.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
css_parser.o: $(BlinkSrc)/core/css/parser/css_parser.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
css_parser_context.o: $(BlinkSrc)/core/css/parser/css_parser_context.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
css_parser_selector.o: $(BlinkSrc)/core/css/parser/css_parser_selector.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
css_parser_token.o: $(BlinkSrc)/core/css/parser/css_parser_token.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
css_parser_token_range.o: $(BlinkSrc)/core/css/parser/css_parser_token_range.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
css_parser_token_stream.o: $(BlinkSrc)/core/css/parser/css_parser_token_stream.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
css_selector_parser.o: $(BlinkSrc)/core/css/parser/css_selector_parser.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
css_tokenizer.o: $(BlinkSrc)/core/css/parser/css_tokenizer.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
css_tokenizer_input_stream.o: $(BlinkSrc)/core/css/parser/css_tokenizer_input_stream.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
selector_checker.o: $(BlinkSrc)/core/css/selector_checker.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
selector_query.o: $(BlinkSrc)/core/css/selector_query.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
attr.o: $(BlinkSrc)/core/dom/attr.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
cdata_section.o: $(BlinkSrc)/core/dom/cdata_section.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
character_data.o: $(BlinkSrc)/core/dom/character_data.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
child_list_mutation_scope.o: $(BlinkSrc)/core/dom/child_list_mutation_scope.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
child_node_list.o: $(BlinkSrc)/core/dom/child_node_list.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
class_collection.o: $(BlinkSrc)/core/dom/class_collection.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
comment.o: $(BlinkSrc)/core/dom/comment.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
container_node.o: $(BlinkSrc)/core/dom/container_node.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
context_lifecycle_notifier.o: $(BlinkSrc)/core/dom/context_lifecycle_notifier.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
context_lifecycle_observer.o: $(BlinkSrc)/core/dom/context_lifecycle_observer.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
decoded_data_document_parser.o: $(BlinkSrc)/core/dom/decoded_data_document_parser.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
document.o: $(BlinkSrc)/core/dom/document.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
document_encoding_data.o: $(BlinkSrc)/core/dom/document_encoding_data.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
document_fragment.o: $(BlinkSrc)/core/dom/document_fragment.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
document_init.o: $(BlinkSrc)/core/dom/document_init.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
document_lifecycle.o: $(BlinkSrc)/core/dom/document_lifecycle.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
document_parser.o: $(BlinkSrc)/core/dom/document_parser.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
document_shutdown_notifier.o: $(BlinkSrc)/core/dom/document_shutdown_notifier.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
document_shutdown_observer.o: $(BlinkSrc)/core/dom/document_shutdown_observer.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
document_type.o: $(BlinkSrc)/core/dom/document_type.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
element.o: $(BlinkSrc)/core/dom/element.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
element_data.o: $(BlinkSrc)/core/dom/element_data.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
element_data_cache.o: $(BlinkSrc)/core/dom/element_data_cache.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
element_rare_data.o: $(BlinkSrc)/core/dom/element_rare_data.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
empty_node_list.o: $(BlinkSrc)/core/dom/empty_node_list.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
add_event_listener_options_resolved.o: $(BlinkSrc)/core/dom/events/add_event_listener_options_resolved.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
event.o: $(BlinkSrc)/core/dom/events/event.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
event_dispatcher.o: $(BlinkSrc)/core/dom/events/event_dispatcher.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
event_dispatch_forbidden_scope.o: $(BlinkSrc)/core/dom/events/event_dispatch_forbidden_scope.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
event_listener_map.o: $(BlinkSrc)/core/dom/events/event_listener_map.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
event_path.o: $(BlinkSrc)/core/dom/events/event_path.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
event_target.o: $(BlinkSrc)/core/dom/events/event_target.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
node_event_context.o: $(BlinkSrc)/core/dom/events/node_event_context.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
registered_event_listener.o: $(BlinkSrc)/core/dom/events/registered_event_listener.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
tree_scope_event_context.o: $(BlinkSrc)/core/dom/events/tree_scope_event_context.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
window_event_context.o: $(BlinkSrc)/core/dom/events/window_event_context.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
id_target_observer_registry.o: $(BlinkSrc)/core/dom/id_target_observer_registry.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
live_node_list_base.o: $(BlinkSrc)/core/dom/live_node_list_base.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
live_node_list_registry.o: $(BlinkSrc)/core/dom/live_node_list_registry.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
mutation_observer_interest_group.o: $(BlinkSrc)/core/dom/mutation_observer_interest_group.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
mutation_record.o: $(BlinkSrc)/core/dom/mutation_record.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
named_node_map.o: $(BlinkSrc)/core/dom/named_node_map.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
node.o: $(BlinkSrc)/core/dom/node.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
node_child_removal_tracker.o: $(BlinkSrc)/core/dom/node_child_removal_tracker.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
node_lists_node_data.o: $(BlinkSrc)/core/dom/node_lists_node_data.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
node_rare_data.o: $(BlinkSrc)/core/dom/node_rare_data.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
node_traversal.o: $(BlinkSrc)/core/dom/node_traversal.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
nth_index_cache.o: $(BlinkSrc)/core/dom/nth_index_cache.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
qualified_name.o: $(BlinkSrc)/core/dom/qualified_name.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
range.o: $(BlinkSrc)/core/dom/range.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
scriptable_document_parser.o: $(BlinkSrc)/core/dom/scriptable_document_parser.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
space_split_string.o: $(BlinkSrc)/core/dom/space_split_string.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
synchronous_mutation_notifier.o: $(BlinkSrc)/core/dom/synchronous_mutation_notifier.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
synchronous_mutation_observer.o: $(BlinkSrc)/core/dom/synchronous_mutation_observer.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
tag_collection.o: $(BlinkSrc)/core/dom/tag_collection.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
text.o: $(BlinkSrc)/core/dom/text.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
tree_ordered_map.o: $(BlinkSrc)/core/dom/tree_ordered_map.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
tree_scope.o: $(BlinkSrc)/core/dom/tree_scope.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
tree_scope_adopter.o: $(BlinkSrc)/core/dom/tree_scope_adopter.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
editing_utilities.o: $(BlinkSrc)/core/editing/editing_utilities.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
markup_accumulator.o: $(BlinkSrc)/core/editing/serializers/markup_accumulator.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
markup_formatter.o: $(BlinkSrc)/core/editing/serializers/markup_formatter.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
serialization.o: $(BlinkSrc)/core/editing/serializers/serialization.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
event_type_names.o: $(BlinkSrc)/core/event_type_names.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
execution_context.o: $(BlinkSrc)/core/execution_context/execution_context.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
web_document_loader_impl.o: $(BlinkSrc)/core/exported/web_document_loader_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
dom_window.o: $(BlinkSrc)/core/frame/dom_window.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
frame.o: $(BlinkSrc)/core/frame/frame.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
frame_lifecycle.o: $(BlinkSrc)/core/frame/frame_lifecycle.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
local_dom_window.o: $(BlinkSrc)/core/frame/local_dom_window.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
local_frame.o: $(BlinkSrc)/core/frame/local_frame.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
location.o: $(BlinkSrc)/core/frame/location.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
navigator.o: $(BlinkSrc)/core/frame/navigator.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
navigator_id.o: $(BlinkSrc)/core/frame/navigator_id.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
navigator_language.o: $(BlinkSrc)/core/frame/navigator_language.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_collection.o: $(BlinkSrc)/core/html/html_collection.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_document.o: $(BlinkSrc)/core/html/html_document.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_tag_collection.o: $(BlinkSrc)/core/html/html_tag_collection.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
atomic_html_token.o: $(BlinkSrc)/core/html/parser/atomic_html_token.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
compact_html_token.o: $(BlinkSrc)/core/html/parser/compact_html_token.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_construction_site.o: $(BlinkSrc)/core/html/parser/html_construction_site.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_document_parser.o: $(BlinkSrc)/core/html/parser/html_document_parser.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_element_stack.o: $(BlinkSrc)/core/html/parser/html_element_stack.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_entity_parser.o: $(BlinkSrc)/core/html/parser/html_entity_parser.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_entity_search.o: $(BlinkSrc)/core/html/parser/html_entity_search.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_formatting_element_list.o: $(BlinkSrc)/core/html/parser/html_formatting_element_list.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_meta_charset_parser.o: $(BlinkSrc)/core/html/parser/html_meta_charset_parser.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_parser_idioms.o: $(BlinkSrc)/core/html/parser/html_parser_idioms.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_parser_options.o: $(BlinkSrc)/core/html/parser/html_parser_options.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_parser_reentry_permit.o: $(BlinkSrc)/core/html/parser/html_parser_reentry_permit.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_preload_scanner.o: $(BlinkSrc)/core/html/parser/html_preload_scanner.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_resource_preloader.o: $(BlinkSrc)/core/html/parser/html_resource_preloader.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_source_tracker.o: $(BlinkSrc)/core/html/parser/html_source_tracker.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_tokenizer.o: $(BlinkSrc)/core/html/parser/html_tokenizer.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_tree_builder.o: $(BlinkSrc)/core/html/parser/html_tree_builder.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_tree_builder_simulator.o: $(BlinkSrc)/core/html/parser/html_tree_builder_simulator.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
preload_request.o: $(BlinkSrc)/core/html/parser/preload_request.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
resource_preloader.o: $(BlinkSrc)/core/html/parser/resource_preloader.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
text_resource_decoder.o: $(BlinkSrc)/core/html/parser/text_resource_decoder.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_element_lookup_trie.o: $(BlinkSrc)/core/html_element_lookup_trie.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_entity_table.o: $(BlinkSrc)/core/html_entity_table.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_names.o: $(BlinkSrc)/core/html_names.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_tokenizer_names.o: $(BlinkSrc)/core/html_tokenizer_names.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
base_fetch_context.o: $(BlinkSrc)/core/loader/base_fetch_context.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
document_loader.o: $(BlinkSrc)/core/loader/document_loader.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
frame_fetch_context.o: $(BlinkSrc)/core/loader/frame_fetch_context.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
frame_loader.o: $(BlinkSrc)/core/loader/frame_loader.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
frame_loader_state_machine.o: $(BlinkSrc)/core/loader/frame_loader_state_machine.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
frame_load_request.o: $(BlinkSrc)/core/loader/frame_load_request.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
navigation_scheduler.o: $(BlinkSrc)/core/loader/navigation_scheduler.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
script_resource.o: $(BlinkSrc)/core/loader/resource/script_resource.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
text_resource.o: $(BlinkSrc)/core/loader/resource/text_resource.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
scheduled_navigation.o: $(BlinkSrc)/core/loader/scheduled_navigation.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
text_resource_decoder_builder.o: $(BlinkSrc)/core/loader/text_resource_decoder_builder.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
classic_pending_script.o: $(BlinkSrc)/core/script/classic_pending_script.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
classic_script.o: $(BlinkSrc)/core/script/classic_script.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
fetch_client_settings_object_impl.o: $(BlinkSrc)/core/script/fetch_client_settings_object_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
html_parser_script_runner.o: $(BlinkSrc)/core/script/html_parser_script_runner.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
pending_script.o: $(BlinkSrc)/core/script/pending_script.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
script_element_base.o: $(BlinkSrc)/core/script/script_element_base.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
script_loader.o: $(BlinkSrc)/core/script/script_loader.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
script_runner.o: $(BlinkSrc)/core/script/script_runner.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
xlink_names.o: $(BlinkSrc)/core/xlink_names.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
xmlns_names.o: $(BlinkSrc)/core/xmlns_names.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
xml_names.o: $(BlinkSrc)/core/xml_names.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
exception_state.o: $(BlinkSrc)/platform/bindings/exception_state.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
gc_pool.o: $(BlinkSrc)/platform/bindings/gc_pool.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
script_forbidden_scope.o: $(BlinkSrc)/platform/bindings/script_forbidden_scope.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
script_wrappers.o: $(BlinkSrc)/platform/bindings/script_wrappers.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
platform.o: $(BlinkSrc)/platform/exported/platform.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
language.o: $(BlinkSrc)/platform/language.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
fetch_context.o: $(BlinkSrc)/platform/loader/fetch/fetch_context.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
fetch_parameters.o: $(BlinkSrc)/platform/loader/fetch/fetch_parameters.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
raw_resource.o: $(BlinkSrc)/platform/loader/fetch/raw_resource.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
resource.o: $(BlinkSrc)/platform/loader/fetch/resource.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
resource_client.o: $(BlinkSrc)/platform/loader/fetch/resource_client.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
resource_error.o: $(BlinkSrc)/platform/loader/fetch/resource_error.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
resource_fetcher.o: $(BlinkSrc)/platform/loader/fetch/resource_fetcher.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
resource_loader.o: $(BlinkSrc)/platform/loader/fetch/resource_loader.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
resource_request.o: $(BlinkSrc)/platform/loader/fetch/resource_request.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
resource_response.o: $(BlinkSrc)/platform/loader/fetch/resource_response.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
source_keyed_cached_metadata_handler.o: $(BlinkSrc)/platform/loader/fetch/source_keyed_cached_metadata_handler.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
text_resource_decoder_options.o: $(BlinkSrc)/platform/loader/fetch/text_resource_decoder_options.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
unique_identifier.o: $(BlinkSrc)/platform/loader/fetch/unique_identifier.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
header_field_tokenizer.o: $(BlinkSrc)/platform/network/header_field_tokenizer.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
http_names.o: $(BlinkSrc)/platform/network/http_names.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
http_parsers.o: $(BlinkSrc)/platform/network/http_parsers.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
content_type.o: $(BlinkSrc)/platform/network/mime/content_type.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
mime_type_registry.o: $(BlinkSrc)/platform/network/mime/mime_type_registry.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
parsed_content_header_field_parameters.o: $(BlinkSrc)/platform/network/parsed_content_header_field_parameters.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
parsed_content_type.o: $(BlinkSrc)/platform/network/parsed_content_type.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
server_timing_header.o: $(BlinkSrc)/platform/network/server_timing_header.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
frame_scheduler_impl.o: $(BlinkSrc)/platform/scheduler/main_thread/frame_scheduler_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
shared_buffer.o: $(BlinkSrc)/platform/shared_buffer.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
segmented_string.o: $(BlinkSrc)/platform/text/segmented_string.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
timer.o: $(BlinkSrc)/platform/timer.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
kurl.o: $(BlinkSrc)/platform/weborigin/kurl.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
security_policy.o: $(BlinkSrc)/platform/weborigin/security_policy.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
web_task_runner.o: $(BlinkSrc)/platform/web_task_runner.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
ascii_ctype.o: $(BlinkSrc)/platform/wtf/ascii_ctype.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
decimal.o: $(BlinkSrc)/platform/wtf/decimal.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
dtoa.o: $(BlinkSrc)/platform/wtf/dtoa.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
bignum-dtoa.o: $(BlinkSrc)/platform/wtf/dtoa/bignum-dtoa.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
bignum.o: $(BlinkSrc)/platform/wtf/dtoa/bignum.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
cached-powers.o: $(BlinkSrc)/platform/wtf/dtoa/cached-powers.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
diy-fp.o: $(BlinkSrc)/platform/wtf/dtoa/diy-fp.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
double-conversion.o: $(BlinkSrc)/platform/wtf/dtoa/double-conversion.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
fast-dtoa.o: $(BlinkSrc)/platform/wtf/dtoa/fast-dtoa.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
fixed-dtoa.o: $(BlinkSrc)/platform/wtf/dtoa/fixed-dtoa.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
strtod.o: $(BlinkSrc)/platform/wtf/dtoa/strtod.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
dynamic_annotations.o: $(BlinkSrc)/platform/wtf/dynamic_annotations.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
hash_table.o: $(BlinkSrc)/platform/wtf/hash_table.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
atomic_string.o: $(BlinkSrc)/platform/wtf/text/atomic_string.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
atomic_string_table.o: $(BlinkSrc)/platform/wtf/text/atomic_string_table.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
cstring.o: $(BlinkSrc)/platform/wtf/text/cstring.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
string_builder.o: $(BlinkSrc)/platform/wtf/text/string_builder.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
string_concatenate.o: $(BlinkSrc)/platform/wtf/text/string_concatenate.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
string_impl.o: $(BlinkSrc)/platform/wtf/text/string_impl.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
string_statics.o: $(BlinkSrc)/platform/wtf/text/string_statics.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
string_to_number.o: $(BlinkSrc)/platform/wtf/text/string_to_number.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
string_view.o: $(BlinkSrc)/platform/wtf/text/string_view.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
text_codec.o: $(BlinkSrc)/platform/wtf/text/text_codec.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
text_codec_iconv.o: $(BlinkSrc)/platform/wtf/text/text_codec_iconv.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
text_codec_latin1.o: $(BlinkSrc)/platform/wtf/text/text_codec_latin1.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
text_codec_replacement.o: $(BlinkSrc)/platform/wtf/text/text_codec_replacement.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
text_codec_user_defined.o: $(BlinkSrc)/platform/wtf/text/text_codec_user_defined.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
text_codec_user_defined_posix.o: $(BlinkSrc)/platform/wtf/text/text_codec_user_defined_posix.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
text_codec_utf16.o: $(BlinkSrc)/platform/wtf/text/text_codec_utf16.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
text_codec_utf8.o: $(BlinkSrc)/platform/wtf/text/text_codec_utf8.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
text_encoding.o: $(BlinkSrc)/platform/wtf/text/text_encoding.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
text_encoding_registry.o: $(BlinkSrc)/platform/wtf/text/text_encoding_registry.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
text_position.o: $(BlinkSrc)/platform/wtf/text/text_position.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
unicode_posix.o: $(BlinkSrc)/platform/wtf/text/unicode_posix.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
utf8.o: $(BlinkSrc)/platform/wtf/text/utf8.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
wtf_string.o: $(BlinkSrc)/platform/wtf/text/wtf_string.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
threading.o: $(BlinkSrc)/platform/wtf/threading.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
time.o: $(BlinkSrc)/platform/wtf/time.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
wtf.o: $(BlinkSrc)/platform/wtf/wtf.cpp
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
wtf_thread_data.o: $(BlinkSrc)/platform/wtf/wtf_thread_data.cc
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
