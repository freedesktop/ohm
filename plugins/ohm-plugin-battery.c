/*
 * Copyright (C) 2007 Richard Hughes <richard@hughsie.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <gmodule.h>
#include <glib.h>

#include "../ohmd/ohm-plugin.h"

enum {
	CONF_SOMETHINGBACKLIGHTCHANGED = 666,
	CONF_LAST
};

static void
load_plugin (OhmPlugin *plugin)
{
	g_debug ("plug:load plugin %p", plugin);
	gint value;
	ohm_plugin_conf_provide (plugin, "battery.percentage");
	ohm_plugin_conf_set_key (plugin, "battery.percentage", 99);
	ohm_plugin_conf_get_key (plugin, "battery.percentage", &value);

	/* these don't have to be one enum per key, you can clump them as classes */
	ohm_plugin_conf_interested (plugin, "backlight.value_foo", CONF_SOMETHINGBACKLIGHTCHANGED);
	ohm_plugin_conf_interested (plugin, "backlight.value_idle", CONF_SOMETHINGBACKLIGHTCHANGED);

	g_debug ("plug:got conf from plugin! %i", value);	
}

static void
unload_plugin (OhmPlugin *plugin)
{
	g_debug ("plug:unload plugin");
}

static void
conf_notify (OhmPlugin *plugin, gint id, gint value)
{
	g_debug ("plug:conf_notify %i: %i", id, value);
	/* using an integer enumeration is much faster than a load of strcmp's */
	if (id == CONF_SOMETHINGBACKLIGHTCHANGED) {
		g_error ("plug:backlight changed, so maybe we need to update something or re-evaluate policy");
	}
}

static OhmPluginInfo plugin_info = {
	"OHM HAL Battery",		/* description */
	"0.0.1",			/* version */
	"richard@hughsie.com",		/* author */
	load_plugin,			/* load */
	unload_plugin,			/* unload */
	conf_notify,			/* conf_notify */
};

OHM_INIT_PLUGIN (plugin_info);
