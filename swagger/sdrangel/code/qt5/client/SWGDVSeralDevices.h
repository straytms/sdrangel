/**
 * SDRangel
 * This is the web API of SDRangel SDR software. SDRangel is an Open Source Qt5/OpenGL 3.0+ GUI and server Software Defined Radio and signal analyzer in software. It supports Airspy, BladeRF, HackRF, LimeSDR, PlutoSDR, RTL-SDR, SDRplay RSP1 and FunCube
 *
 * OpenAPI spec version: 4.0.0
 * Contact: f4exb06@gmail.com
 *
 * NOTE: This class is auto generated by the swagger code generator program.
 * https://github.com/swagger-api/swagger-codegen.git
 * Do not edit the class manually.
 */

/*
 * SWGDVSeralDevices.h
 * 
 * List of DV serial devices available in the system
 */

#ifndef SWGDVSeralDevices_H_
#define SWGDVSeralDevices_H_

#include <QJsonObject>


#include "SWGDVSerialDevice.h"
#include <QList>

#include "SWGObject.h"


namespace Swagger {

class SWGDVSeralDevices: public SWGObject {
public:
    SWGDVSeralDevices();
    SWGDVSeralDevices(QString* json);
    virtual ~SWGDVSeralDevices();
    void init();
    void cleanup();

    QString asJson ();
    QJsonObject* asJsonObject();
    void fromJsonObject(QJsonObject &json);
    SWGDVSeralDevices* fromJson(QString &jsonString);

    qint32 getNbDevices();
    void setNbDevices(qint32 nb_devices);

    QList<SWGDVSerialDevice*>* getDvSerialDevices();
    void setDvSerialDevices(QList<SWGDVSerialDevice*>* dv_serial_devices);


private:
    qint32 nb_devices;
    QList<SWGDVSerialDevice*>* dv_serial_devices;
};

}

#endif /* SWGDVSeralDevices_H_ */
