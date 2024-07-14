/*
 * Copyright © 2020 UBports foundation
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
 */

#include "sensorfw_proximity_sensor.h"
#include "event_loop_handler_registration.h"

#include "socketreader.h"

namespace
{
auto const null_handler = [](sensorfw_proxy::ProximityState){};
}

sensorfw_proxy::SensorfwProximitySensor::SensorfwProximitySensor(
    std::shared_ptr<Log> const& log,
    std::string const& dbus_bus_address)
    : Sensorfw(log, dbus_bus_address, "Proximity", PluginType::PROXIMITY),
      m_handler{null_handler},
      m_state{ProximityState::far}
{
}

sensorfw_proxy::HandlerRegistration sensorfw_proxy::SensorfwProximitySensor::register_proximity_handler(
    ProximityHandler const& handler)
{
    return EventLoopHandlerRegistration{
        dbus_event_loop,
        [this, &handler]{ this->m_handler = handler; },
        [this]{ this->m_handler = null_handler; }};
}

void sensorfw_proxy::SensorfwProximitySensor::enable_proximity_events()
{
    dbus_event_loop.enqueue(
        [this]
        {
            start();
        }).get();
}

void sensorfw_proxy::SensorfwProximitySensor::disable_proximity_events()
{
    dbus_event_loop.enqueue(
        [this]
        {
            stop();
        }).get();
}

void sensorfw_proxy::SensorfwProximitySensor::data_recived_impl()
{
    QVector<ProximityData> values;
    if (m_socket->read<ProximityData>(values))
        m_state = values[0].withinProximity_ ? ProximityState::near : ProximityState::far;
    else
        m_state = ProximityState::far;

    m_handler(m_state);
}

sensorfw_proxy::ProximityState sensorfw_proxy::SensorfwProximitySensor::proximity_state()
{
    return m_state;
}
