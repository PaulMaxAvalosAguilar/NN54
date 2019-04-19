#ifndef BLESCANNERMODEL_H
#define BLESCANNERMODEL_H

#include <QAbstractListModel>
#include <QBluetoothDeviceDiscoveryAgent>
#include <memory>
#include <vector>
#include "deviceinfo.h"
#include "connectionhandling.h"

class BleScannerModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString scannerState READ scannerState WRITE setScannerState NOTIFY scannerStateChanged)
    Q_PROPERTY(bool scanning READ getScanning WRITE setScanning NOTIFY scanningChanged)

    Q_INVOKABLE void create(const QModelIndex &index, const DeviceInfo &device);
    Q_INVOKABLE bool removeRows(const QModelIndex &index, int count, const QModelIndex &parent);

    Q_INVOKABLE void append(const DeviceInfo &device);
    Q_INVOKABLE void clear();

public:
    enum Roles{
        BleNameRole = Qt::UserRole +1,
        BleAdressRole,
        BleRssi,
    };

    BleScannerModel(QObject *parent= 0);
    ~BleScannerModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString scannerState() const;
    void setScannerState(const QString &scannerState);
    bool getScanning() const;
    void setScanning(bool value);

    std::unique_ptr<ConnectionHandling> &getConnHandling();

signals:
    void scannerStateChanged();
    void scanningChanged();

public slots:
    void startDiscovery();
    void connectToDevice(int index);

private slots:
    // QBluetoothDeviceDiscoveryAgent related
    void addDevice(const QBluetoothDeviceInfo&);
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error);
    void deviceScanFinished();

private:
     bool isIndexValid(const QModelIndex& index) const;

private:
    QBluetoothDeviceDiscoveryAgent *bleAgent;
    std::unique_ptr<std::vector<std::unique_ptr<DeviceInfo>>> devicesInfo;
    std::unique_ptr<ConnectionHandling> connHandling;
    QString m_scannerState;
    bool scanning;
};

#endif // BLESCANNERMODEL_H
