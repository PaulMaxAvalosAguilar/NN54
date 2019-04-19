TEMPLATE = subdirs

SUBDIRS += \
    gallery-core \
    QmlApp

QmlApp.depends=gallery-core
