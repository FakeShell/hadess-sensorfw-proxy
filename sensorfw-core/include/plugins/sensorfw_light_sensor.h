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

#pragma once

#include <utils/handler_registration.h>
#include <plugins/sensorfw_common.h>
#include <datatypes/timedunsigned.h>

namespace sensorfw_proxy
{
namespace core
{

using LightHandler = std::function<void(TimedUnsigned)>;

class SensorfwLightSensor : public Sensorfw
{
public:
    SensorfwLightSensor(std::string const& dbus_bus_address);

    HandlerRegistration register_light_handler(LightHandler const& handler);

    void enable_light_events();
    void disable_light_events();
private:
    void data_recived_impl();

    LightHandler handler;
};

}
}