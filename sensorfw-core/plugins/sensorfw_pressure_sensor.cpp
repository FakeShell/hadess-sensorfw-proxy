/*
 * Copyright © 2020 UBports foundation
 * Copyright © 2021 Waydroid Project.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Marius Gripsgard <marius@ubports.com>
 * Authored by: Erfan Abdi <erfangplus@gmail.com>
 */

#include <plugins/sensorfw_pressure_sensor.h>

#include <stdexcept>

namespace
{
auto const null_handler = [](TimedUnsigned){};
}

sensorfw_proxy::core::SensorfwPressureSensor::SensorfwPressureSensor(
    std::string const &dbus_bus_address)
    : Sensorfw(dbus_bus_address, "Pressure", PluginType::PRESSURE),
      handler{null_handler}
{
}

sensorfw_proxy::core::HandlerRegistration sensorfw_proxy::core::SensorfwPressureSensor::register_pressure_handler(
    PressureHandler const& handler)
{
    return EventLoopHandlerRegistration{
        dbus_event_loop,
        [this, &handler]{ this->handler = handler; },
        [this]{ this->handler = null_handler; }};
}

void sensorfw_proxy::core::SensorfwPressureSensor::enable_pressure_events()
{
    dbus_event_loop.enqueue(
        [this]
        {
            start();
        }).get();
}

void sensorfw_proxy::core::SensorfwPressureSensor::disable_pressure_events()
{
    dbus_event_loop.enqueue(
        [this]
        {
            stop();
        }).get();
}

void sensorfw_proxy::core::SensorfwPressureSensor::data_recived_impl()
{
    std::vector<TimedUnsigned> values;
    if(!m_socket->read<TimedUnsigned>(values))
        return;

    handler(values[0]);
}
