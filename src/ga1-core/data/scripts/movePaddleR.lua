function update (component, frame_params)
	if frame_params_get_input_up_R(frame_params) then
		entity = component_get_entity(component)
		entity_translate(entity, 0.0, 1.0, 0.0)
	end
	if frame_params_get_input_down_R(frame_params) then
		entity = component_get_entity(component)
		entity_translate(entity, 0.0, -1.0, 0.0)
	end
end
