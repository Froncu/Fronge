#ifndef FRONGE_HPP
#define FRONGE_HPP

#include "application.hpp"
#include "constants.hpp"
#include "core.hpp"
#include "events/event.hpp"
#include "events/observer/dispatchable.hpp"
#include "events/observer/event_dispatcher.hpp"
#include "events/observer/event_listener.hpp"
#include "events/render_context_event.hpp"
#include "froch.hpp"
#include "identifier/id.hpp"
#include "identifier/id_generator.hpp"
#include "input/gamepad.hpp"
#include "input/input.hpp"
#include "input/input_manager.hpp"
#include "maths/arithmetic.hpp"
#include "maths/matrix.hpp"
#include "maths/rectangle.hpp"
#include "maths/resulting_component.hpp"
#include "maths/transform_matrix.hpp"
#include "maths/vector2.hpp"
#include "maths/vector3.hpp"
#include "reference/base_reference.hpp"
#include "reference/reference.hpp"
#include "reference/referenceable.hpp"
#include "render_context/render_context.hpp"
#include "render_context/texture.hpp"
#include "resources/surface/surface.hpp"
#include "services/locator.hpp"
#include "services/logger/coloured_logger.hpp"
#include "services/logger/logger.hpp"
#include "services/system_event_dispatcher/regular_system_event_dispatcher.hpp"
#include "services/system_event_dispatcher/system_event_dispatcher.hpp"
#include "utility/assert.hpp"
#include "utility/exception.hpp"
#include "utility/lazy_hasher.hpp"
#include "utility/type_index.hpp"
#include "utility/unique_pointer.hpp"
#include "utility/variant_visitor.hpp"
#include "utility/void_deleter.hpp"

#endif