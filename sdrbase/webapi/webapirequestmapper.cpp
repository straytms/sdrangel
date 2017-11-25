///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017 Edouard Griffiths, F4EXB.                                  //
//                                                                               //
// Swagger server adapter interface                                              //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

//#include <QDirIterator>
#include <QJsonDocument>
#include <QJsonArray>

#include "httpdocrootsettings.h"
#include "webapirequestmapper.h"
#include "SWGInstanceSummaryResponse.h"
#include "SWGInstanceDevicesResponse.h"
#include "SWGInstanceChannelsResponse.h"
#include "SWGAudioDevices.h"
#include "SWGAudioDevicesSelect.h"
#include "SWGLocationInformation.h"
#include "SWGDVSeralDevices.h"
#include "SWGErrorResponse.h"

WebAPIRequestMapper::WebAPIRequestMapper(QObject* parent) :
    HttpRequestHandler(parent),
    m_adapter(0)
{
    qtwebapp::HttpDocrootSettings docrootSettings;
    docrootSettings.path = ":/";
    m_staticFileController = new qtwebapp::StaticFileController(docrootSettings, parent);
}

WebAPIRequestMapper::~WebAPIRequestMapper()
{
    delete m_staticFileController;
}

void WebAPIRequestMapper::service(qtwebapp::HttpRequest& request, qtwebapp::HttpResponse& response)
{
    if (m_adapter == 0) // format service unavailable if adapter is null
    {
        response.setStatus(500,"Service not available");
        response.write("Service not available");
    }
    else // normal processing
    {
        QByteArray path=request.getPath();

        if (path == WebAPIAdapterInterface::instanceSummaryURL) {
            instanceSummaryService(request, response);
        } else if (path == WebAPIAdapterInterface::instanceDevicesURL) {
            instanceDevicesService(request, response);
        } else if (path == WebAPIAdapterInterface::instanceChannelsURL) {
            instanceChannelsService(request, response);
        } else if (path == WebAPIAdapterInterface::instanceLoggingURL) {
            instanceLoggingService(request, response);
        } else if (path == WebAPIAdapterInterface::instanceAudioURL) {
            instanceAudioService(request, response);
        } else if (path == WebAPIAdapterInterface::instanceLocationURL) {
            instanceLocationService(request, response);
        } else if (path == WebAPIAdapterInterface::instanceDVSerialURL) {
            instanceDVSerialService(request, response);
        }
        else
        {
//            QDirIterator it(":", QDirIterator::Subdirectories);
//            while (it.hasNext()) {
//                qDebug() << "WebAPIRequestMapper::service: " << it.next();
//            }

            QByteArray path = "/index.html";
            m_staticFileController->service(path, response);
        }
    }
}

void WebAPIRequestMapper::instanceSummaryService(qtwebapp::HttpRequest& request, qtwebapp::HttpResponse& response)
{
    if (request.getMethod() == "GET")
    {
        Swagger::SWGInstanceSummaryResponse normalResponse;
        Swagger::SWGErrorResponse errorResponse;

        int status = m_adapter->instanceSummary(normalResponse, errorResponse);
        response.setStatus(status);

        if (status == 200) {
            response.write(normalResponse.asJson().toUtf8());
        } else {
            response.write(errorResponse.asJson().toUtf8());
        }
    }
    else
    {
        response.setStatus(405,"Invalid HTTP method");
        response.write("Invalid HTTP method");
    }
}

void WebAPIRequestMapper::instanceDevicesService(qtwebapp::HttpRequest& request, qtwebapp::HttpResponse& response)
{
    Swagger::SWGInstanceDevicesResponse normalResponse;
    Swagger::SWGErrorResponse errorResponse;

    if (request.getMethod() == "GET")
    {
        QByteArray txStr = request.getParameter("tx");
        bool tx = false;

        if (txStr.length() != 0) {
            tx = !(txStr == "0");
        }

        int status = m_adapter->instanceDevices(tx, normalResponse, errorResponse);
        response.setStatus(status);

        if (status == 200) {
            response.write(normalResponse.asJson().toUtf8());
        } else {
            response.write(errorResponse.asJson().toUtf8());
        }
    }
    else
    {
        response.setStatus(405,"Invalid HTTP method");
        response.write("Invalid HTTP method");
    }
}

void WebAPIRequestMapper::instanceChannelsService(qtwebapp::HttpRequest& request, qtwebapp::HttpResponse& response)
{
    Swagger::SWGInstanceChannelsResponse normalResponse;
    Swagger::SWGErrorResponse errorResponse;

    if (request.getMethod() == "GET")
    {
        QByteArray txStr = request.getParameter("tx");
        bool tx = false;

        if (txStr.length() != 0) {
            tx = !(txStr == "0");
        }

        int status = m_adapter->instanceChannels(tx, normalResponse, errorResponse);
        response.setStatus(status);

        if (status == 200) {
            response.write(normalResponse.asJson().toUtf8());
        } else {
            response.write(errorResponse.asJson().toUtf8());
        }
    }
    else
    {
        response.setStatus(405,"Invalid HTTP method");
        response.write("Invalid HTTP method");
    }
}

void WebAPIRequestMapper::instanceLoggingService(qtwebapp::HttpRequest& request, qtwebapp::HttpResponse& response)
{
    Swagger::SWGLoggingInfo normalResponse;
    Swagger::SWGErrorResponse errorResponse;

    if (request.getMethod() == "GET")
    {
        int status = m_adapter->instanceLoggingGet(normalResponse, errorResponse);
        response.setStatus(status);

        if (status == 200) {
            response.write(normalResponse.asJson().toUtf8());
        } else {
            response.write(errorResponse.asJson().toUtf8());
        }
    }
    else if (request.getMethod() == "PUT")
    {
        QString jsonStr = request.getBody();

        if (parseJsonBody(jsonStr, response))
        {
            normalResponse.fromJson(jsonStr);
            int status = m_adapter->instanceLoggingPut(normalResponse, errorResponse);
            response.setStatus(status);

            if (status == 200) {
                response.write(normalResponse.asJson().toUtf8());
            } else {
                response.write(errorResponse.asJson().toUtf8());
            }
        }
    }
    else
    {
        response.setStatus(405,"Invalid HTTP method");
        response.write("Invalid HTTP method");
    }
}

void WebAPIRequestMapper::instanceAudioService(qtwebapp::HttpRequest& request, qtwebapp::HttpResponse& response)
{
    Swagger::SWGErrorResponse errorResponse;

    if (request.getMethod() == "GET")
    {
        Swagger::SWGAudioDevices normalResponse;

        int status = m_adapter->instanceAudioGet(normalResponse, errorResponse);
        response.setStatus(status);

        if (status == 200) {
            response.write(normalResponse.asJson().toUtf8());
        } else {
            response.write(errorResponse.asJson().toUtf8());
        }
    }
    else if (request.getMethod() == "PATCH")
    {
        Swagger::SWGAudioDevicesSelect normalResponse;
        QString jsonStr = request.getBody();

        if (parseJsonBody(jsonStr, response))
        {
            normalResponse.fromJson(jsonStr);
            int status = m_adapter->instanceAudioPatch(normalResponse, errorResponse);
            response.setStatus(status);

            if (status == 200) {
                response.write(normalResponse.asJson().toUtf8());
            } else {
                response.write(errorResponse.asJson().toUtf8());
            }
        }
    }
    else
    {
        response.setStatus(405,"Invalid HTTP method");
        response.write("Invalid HTTP method");
    }
}

void WebAPIRequestMapper::instanceLocationService(qtwebapp::HttpRequest& request, qtwebapp::HttpResponse& response)
{
    Swagger::SWGErrorResponse errorResponse;

    if (request.getMethod() == "GET")
    {
        Swagger::SWGLocationInformation normalResponse;

        int status = m_adapter->instanceLocationGet(normalResponse, errorResponse);
        response.setStatus(status);

        if (status == 200) {
            response.write(normalResponse.asJson().toUtf8());
        } else {
            response.write(errorResponse.asJson().toUtf8());
        }
    }
    else if (request.getMethod() == "PUT")
    {
        Swagger::SWGLocationInformation normalResponse;
        QString jsonStr = request.getBody();

        if (parseJsonBody(jsonStr, response))
        {
            normalResponse.fromJson(jsonStr);
            int status = m_adapter->instanceLocationPut(normalResponse, errorResponse);
            response.setStatus(status);

            if (status == 200) {
                response.write(normalResponse.asJson().toUtf8());
            } else {
                response.write(errorResponse.asJson().toUtf8());
            }
        }
    }
    else
    {
        response.setStatus(405,"Invalid HTTP method");
        response.write("Invalid HTTP method");
    }
}

void WebAPIRequestMapper::instanceDVSerialService(qtwebapp::HttpRequest& request, qtwebapp::HttpResponse& response)
{
    Swagger::SWGErrorResponse errorResponse;

    if (request.getMethod() == "PATCH")
    {
        QByteArray dvserialStr = request.getParameter("dvserial");
        bool dvserial = false;

        if (dvserialStr.length() != 0) {
            dvserial = !(dvserialStr == "0");
        }

        Swagger::SWGDVSeralDevices normalResponse;

        int status = m_adapter->instanceDVSerialPatch(dvserial, normalResponse, errorResponse);
        response.setStatus(status);

        if (status == 200) {
            response.write(normalResponse.asJson().toUtf8());
        } else {
            response.write(errorResponse.asJson().toUtf8());
        }
    }
    else
    {
        response.setStatus(405,"Invalid HTTP method");
        response.write("Invalid HTTP method");
    }
}

bool WebAPIRequestMapper::parseJsonBody(QString& jsonStr, qtwebapp::HttpResponse& response)
{
    Swagger::SWGErrorResponse errorResponse;

    try
    {
        QByteArray jsonBytes(jsonStr.toStdString().c_str());
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(jsonBytes, &error);

        if (error.error != QJsonParseError::NoError)
        {
            QString errorMsg = QString("Input JSON error: ") + error.errorString() + QString(" at offset ") + QString::number(error.offset);
            errorResponse.init();
            *errorResponse.getMessage() = errorMsg;
            response.setStatus(400, errorMsg.toUtf8());
            response.write(errorResponse.asJson().toUtf8());
        }

        return (error.error == QJsonParseError::NoError);
    }
    catch (const std::exception& ex)
    {
        QString errorMsg = QString("Error parsing request: ") + ex.what();
        errorResponse.init();
        *errorResponse.getMessage() = errorMsg;
        response.setStatus(500, errorMsg.toUtf8());
        response.write(errorResponse.asJson().toUtf8());

        return false;
    }
}
