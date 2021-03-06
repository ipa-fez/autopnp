/*
 * Copyright (c) 2011-2014, fortiss GmbH.
 * Licensed under the Apache License, Version 2.0.
 *
 * Use, modification and distribution are subject to the terms specified
 * in the accompanying license file LICENSE.txt located at the root directory
 * of this software distribution. A copy is available at
 * http://chromosome.fortiss.org/.
 *
 * This file is part of CHROMOSOME.
 *
 * $Id$
 */

/**
 * \file
 *         Source file for capabilitiesView component.
 *
 * \author
 *         This file has been generated by the CHROMOSOME Modeling Tool (XMT)
 *         (fortiss GmbH).
 */

/******************************************************************************/
/***   Includes                                                             ***/
/******************************************************************************/
#include "autoPnP/adv/capabilitiesView/include/capabilitiesViewComponent.h"

// PROTECTED REGION ID(AUTOPNP_ADV_CAPABILITIESVIEW_CAPABILITIESVIEWCOMPONENT_C_INCLUDES) ENABLED START
//#include <direct.h>
#include <fstream>
#include <iostream>

#include "xme/hal/include/graphviz.h" //TODO
#include "xme/hal/include/qt.h" //TODO
#include "xme/hal/include/safeString.h" //TODO

#ifndef WIN32
#include <sys/stat.h>
#define _MAX_PATH 4096
#define _getcwd getcwd
#define _mkdir(x) mkdir(x, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH)
#endif

using namespace std;
// PROTECTED REGION END

/******************************************************************************/
/***   Defines                                                              ***/
/******************************************************************************/

// PROTECTED REGION ID(AUTOPNP_ADV_CAPABILITIESVIEW_CAPABILITIESVIEWCOMPONENT_C_DEFINES) ENABLED START
// PROTECTED REGION END

/******************************************************************************/
/***   Type definitions                                                     ***/
/******************************************************************************/

// PROTECTED REGION ID(AUTOPNP_ADV_CAPABILITIESVIEW_CAPABILITIESVIEWCOMPONENT_C_TYPE_DEFINITIONS) ENABLED START
// PROTECTED REGION END

/******************************************************************************/
/***   Variables                                                            ***/
/******************************************************************************/

// PROTECTED REGION ID(AUTOPNP_ADV_CAPABILITIESVIEW_CAPABILITIESVIEWCOMPONENT_C_VARIABLES) ENABLED START

//ugly shit
static std::string _UGLY_config;
// PROTECTED REGION END

/******************************************************************************/
/***   Prototypes                                                           ***/
/******************************************************************************/

// PROTECTED REGION ID(AUTOPNP_ADV_CAPABILITIESVIEW_CAPABILITIESVIEWCOMPONENT_C_PROTOTYPES) ENABLED START
static void autoPnP_adv_capabilitiesView_capabilitiesViewComponent_createMainWindow(void* userData);
static void autoPnP_adv_capabilitiesView_capabilitiesViewComponent_task(void* userData);
static void autoPnP_adv_capabilitiesView_capabilitiesViewComponent_updateCapabilitiesView(autoPnP_adv_capabilitiesView_capabilitiesViewComponent_config_t* const config);
static void autoPnP_adv_capabilitiesView_capabilitiesViewComponent_generateChilds(std::string& strConns, std::string &strNodes, int indentationLevel, autoPnP_adv_capabilitiesView_capabilitiesViewComponent_config_t* const config, xme_hal_time_timeHandle_t currentTimeHandle, const char* parent = "", const char* parentPath = "");
static void autoPnP_adv_capabilitiesView_capabilitiesViewComponent_createIntendation(ofstream& dotFile, int indentationLevel);
// PROTECTED REGION END

/******************************************************************************/
/***   Implementation                                                       ***/
/******************************************************************************/
xme_status_t
autoPnP_adv_capabilitiesView_capabilitiesViewComponent_init
(
    autoPnP_adv_capabilitiesView_capabilitiesViewComponent_config_t* const config,
    const char* initializationString
)
{
    // PROTECTED REGION ID(AUTOPNP_ADV_CAPABILITIESVIEW_CAPABILITIESVIEWCOMPONENT_C_INITIALIZE) ENABLED START
	XME_UNUSED_PARAMETER(initializationString);

	char buffer[_MAX_PATH];
	_getcwd(buffer, sizeof(buffer));
//	config->directory = std::string(buffer) + "/AutoPnP";
	_UGLY_config = std::string(buffer) + "/AutoPnP/www";
	_mkdir((std::string(buffer) + "/AutoPnP/www").c_str());

	XME_HAL_TABLE_INIT(config->components);

	config->taskHandle = xme_hal_sched_addTask(xme_hal_time_timeIntervalFromMilliseconds(3000), xme_hal_time_timeIntervalFromMilliseconds(500), 0, autoPnP_adv_capabilitiesView_capabilitiesViewComponent_task, config);
	// Check for errors
	XME_CHECK(XME_HAL_SCHED_INVALID_TASK_HANDLE != config->taskHandle, XME_STATUS_INTERNAL_ERROR);

	return XME_STATUS_SUCCESS;
    // PROTECTED REGION END
}

void
autoPnP_adv_capabilitiesView_capabilitiesViewComponent_fini
(
    autoPnP_adv_capabilitiesView_capabilitiesViewComponent_config_t* const config
)
{
    // PROTECTED REGION ID(AUTOPNP_ADV_CAPABILITIESVIEW_CAPABILITIESVIEWCOMPONENT_C_FINALIZE) ENABLED START
	delete config->capabilitiesWindow;

	XME_HAL_TABLE_FINI(config->components);
    // PROTECTED REGION END
}

// PROTECTED REGION ID(AUTOPNP_ADV_CAPABILITIESVIEW_CAPABILITIESVIEWCOMPONENT_C_IMPLEMENTATION) ENABLED START
static void autoPnP_adv_capabilitiesView_capabilitiesViewComponent_createMainWindow(void* userData)
{
	autoPnP_adv_capabilitiesView_capabilitiesViewComponent_config_t* config = (autoPnP_adv_capabilitiesView_capabilitiesViewComponent_config_t*)userData;

	config->capabilitiesWindow = new CapabilitiesWindow("file://"+_UGLY_config + "/view.html");
	autoPnP_adv_capabilitiesView_capabilitiesViewComponent_updateCapabilitiesView(config);
	config->capabilitiesWindow->show();
}

static void autoPnP_adv_capabilitiesView_capabilitiesViewComponent_task(void* userData)
{
	static bool initialized = false;

	autoPnP_adv_capabilitiesView_capabilitiesViewComponent_config_t* config = (autoPnP_adv_capabilitiesView_capabilitiesViewComponent_config_t*)userData;

	if (!initialized)
	{
		xme_hal_qt_triggerExecution(autoPnP_adv_capabilitiesView_capabilitiesViewComponent_createMainWindow, config);

		initialized = true;
	}
	else
	{
		autoPnP_adv_capabilitiesView_capabilitiesViewComponent_updateCapabilitiesView(config);
	}
}

static bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

static void autoPnP_adv_capabilitiesView_capabilitiesViewComponent_updateCapabilitiesView(autoPnP_adv_capabilitiesView_capabilitiesViewComponent_config_t* const config)
{
	ofstream viewFile;
	ifstream tempFile;
	std::string filepath = _UGLY_config + "/view.html";
	std::string templatepath = _UGLY_config + "/template.html";

	tempFile.open(templatepath.c_str());
	viewFile.open(filepath.c_str());

	static std::string old_content;
	std::string content((std::istreambuf_iterator<char>(tempFile)), std::istreambuf_iterator<char>()), strConns, strNodes;
	xme_hal_time_timeHandle_t currentTimeHandle = xme_hal_time_getCurrentTime();
	autoPnP_adv_capabilitiesView_capabilitiesViewComponent_generateChilds(strConns, strNodes, 1, config, currentTimeHandle);

	replace(content, "$CONNS$", strConns);
	replace(content, "$NODES$", strNodes);
	viewFile << content;

	viewFile.close();
	tempFile.close();

	if(old_content!=content)
		config->capabilitiesWindow->updateCapabilities();
	old_content=content;
/*
	ofstream dotFile;
	std::string filepath = _UGLY_config + "/capabilitiesView.dot";

	xme_hal_time_timeHandle_t currentTimeHandle = xme_hal_time_getCurrentTime();

	dotFile.open(filepath.c_str());
	dotFile << "digraph G {" << endl;
	autoPnP_adv_capabilitiesView_capabilitiesViewComponent_generateChilds(dotFile, 1, config, currentTimeHandle);
	dotFile << "};" << endl;
	dotFile.close();

	xme_hal_graphviz_generateImage(filepath.c_str(), (_UGLY_config + "/capabilitiesView.png").c_str());

	config->capabilitiesWindow->updateCapabilities();
*/
}


static void autoPnP_adv_capabilitiesView_capabilitiesViewComponent_generateChilds(std::string& strConns, std::string &strNodes, int indentationLevel, autoPnP_adv_capabilitiesView_capabilitiesViewComponent_config_t* const config, xme_hal_time_timeHandle_t currentTimeHandle, const char* parent, const char*parentPath)
{
	bool restart;
	int pos=0;

	do
	{
		restart = false;

		XME_HAL_TABLE_ITERATE_BEGIN
		(
			config->components,
			xme_hal_table_rowHandle_t, handle,
			autoPnP_adv_capabilitiesView_capabilitiesViewComponent_component_t, component
		);
		{
			if (!strcmp(component->component.parent, parent)) //TODO
			{
				if (COMPONENT_STATUS_ACTIVE != component->status)
				{
					if (xme_hal_time_timeIntervalInSeconds(xme_hal_time_getTimeIntervalBetween(component->timeHandle, currentTimeHandle)) >= 3)
					{
						if (COMPONENT_STATUS_ADDED == component->status)
						{
							component->status = COMPONENT_STATUS_ACTIVE;
						}
						else
						{
							XME_HAL_TABLE_REMOVE_ITEM(config->components, handle);
							restart = true;
							break;
						}
					}
				}

				std::string preparedName, preparedName2, parent_name = component->component.parent;

				if(parent_name.size()<1)
					strConns += std::string("instance.connect({ source:\"sourceWindow1\", target:\"")+component->component.name+"\" });";
				else
					strConns += std::string("instance.connect({ source:\""+parent_name+"\", target:\"")+component->component.name+"\" });";

				if (strcmp(parentPath, "")) //TODO
				{
					preparedName = parentPath;
					preparedName += "_";
				}
				else
				{
					preparedName = "";
				}
				preparedName += component->component.name;
				preparedName2 = preparedName;
				replace(preparedName2.begin(), preparedName2.end(), '_', '.');

				std::stringstream ss;
				ss<<"top:25em; left:"<<((pos)*20+2)<<"em; height:"<<(std::max((int)component->component.capabilitiesCount,1)+1)*5<<"em;";
				++pos;

				std::string style=ss.str(), capap;
				switch (component->status)
				{
					case COMPONENT_STATUS_ADDED:
						style += "color:green;";
						break;
					case COMPONENT_STATUS_REMOVED:
						style += "color:red;";
						break;
					default:break;
				}

				if (0 < component->component.capabilitiesCount)
				{
					for (int i = 0; i < component->component.capabilitiesCount; i++)
						capap += "<p>"+preparedName2+"."+component->component.capabilities[i]+"</p>";
				}
				else
				{
					capap += "<p>"+preparedName2+".Empty</p>";
				}

				strNodes += std::string("<div class='window smallWindow' id='")+component->component.name+"' style='"+style+"'><strong>"+preparedName2+"</strong>"+capap+"<br/></div>";
			}
		}
		XME_HAL_TABLE_ITERATE_END();
	} while (restart);
}

/*
static void autoPnP_adv_capabilitiesView_capabilitiesViewComponent_generateChilds(ofstream& dotFile, int indentationLevel, autoPnP_adv_capabilitiesView_capabilitiesViewComponent_config_t* const config, xme_hal_time_timeHandle_t currentTimeHandle, const char* parent, const char*parentPath)
{
	bool restart;

	do
	{
		restart = false;

		XME_HAL_TABLE_ITERATE_BEGIN
		(
			config->components,
			xme_hal_table_rowHandle_t, handle,
			autoPnP_adv_capabilitiesView_capabilitiesViewComponent_component_t, component
		);
		{
			if (!strcmp(component->component.parent, parent)) //TODO
			{
				if (COMPONENT_STATUS_ACTIVE != component->status)
				{
					if (xme_hal_time_timeIntervalInSeconds(xme_hal_time_getTimeIntervalBetween(component->timeHandle, currentTimeHandle)) >= 3)
					{
						if (COMPONENT_STATUS_ADDED == component->status)
						{
							component->status = COMPONENT_STATUS_ACTIVE;
						}
						else
						{
							XME_HAL_TABLE_REMOVE_ITEM(config->components, handle);
							restart = true;
							break;
						}
					}
				}

				std::string preparedName;
				std::string preparedName2;

				if (strcmp(parentPath, "")) //TODO
				{
					preparedName = parentPath;
					preparedName += "_";
				}
				else
				{
					preparedName = "";
				}
				preparedName += component->component.name;
				preparedName2 = preparedName;
				replace(preparedName2.begin(), preparedName2.end(), '_', '.');

				autoPnP_adv_capabilitiesView_capabilitiesViewComponent_createIntendation(dotFile, indentationLevel);
				dotFile << "subgraph cluster" << preparedName << " {" << endl;

				autoPnP_adv_capabilitiesView_capabilitiesViewComponent_createIntendation(dotFile, indentationLevel + 1);
				dotFile << "label=\"" << preparedName2 << "\";" << endl;
				autoPnP_adv_capabilitiesView_capabilitiesViewComponent_createIntendation(dotFile, indentationLevel + 1);
				dotFile << "node [style=filled];" << endl;
				autoPnP_adv_capabilitiesView_capabilitiesViewComponent_createIntendation(dotFile, indentationLevel + 1);
				dotFile << "style=filled;" << endl;
				autoPnP_adv_capabilitiesView_capabilitiesViewComponent_createIntendation(dotFile, indentationLevel + 1);
				switch (component->status)
				{
					case COMPONENT_STATUS_ADDED:
						dotFile << "color=green;" << endl;
						break;
					case COMPONENT_STATUS_REMOVED:
						dotFile << "color=red;" << endl;
						break;
					default:
						dotFile << "color=black;" << endl;
						break;
				}
				autoPnP_adv_capabilitiesView_capabilitiesViewComponent_createIntendation(dotFile, indentationLevel + 1);
				dotFile << "fillcolor=white;" << endl;
				if (0 < component->component.capabilitiesCount)
				{
					for (int i = 0; i < component->component.capabilitiesCount; i++)
					{
						autoPnP_adv_capabilitiesView_capabilitiesViewComponent_createIntendation(dotFile, indentationLevel + 1);
						dotFile << "\"" << preparedName2 << "." << component->component.capabilities[i] << "\";" << endl;
						if (0 < i)
						{
							autoPnP_adv_capabilitiesView_capabilitiesViewComponent_createIntendation(dotFile, indentationLevel + 1);
							dotFile << "\"" << preparedName2 << "." << component->component.capabilities[i - 1] << "\" -> \"" << preparedName2 << "." << component->component.capabilities[i] << "\" [style=invis];" << endl;
						}
					}
				}
				else
				{
					autoPnP_adv_capabilitiesView_capabilitiesViewComponent_createIntendation(dotFile, indentationLevel + 1);
					dotFile << "\"" << preparedName2 << ".Empty\";" << endl;
				}

				autoPnP_adv_capabilitiesView_capabilitiesViewComponent_generateChilds(dotFile, indentationLevel + 1, config, currentTimeHandle, component->component.name, preparedName.c_str());

				autoPnP_adv_capabilitiesView_capabilitiesViewComponent_createIntendation(dotFile, indentationLevel);
				dotFile << "}" << endl;
			}
		}
		XME_HAL_TABLE_ITERATE_END();
	} while (restart);
}
*/

static void autoPnP_adv_capabilitiesView_capabilitiesViewComponent_createIntendation(ofstream& dotFile, int indentationLevel)
{
	for (int i = 0; i < indentationLevel; i++)
	{
		dotFile << "\t";
	}
}
// PROTECTED REGION END
