/*
 * Copyright (c) 2013 BlackBerry Limited
 * Author: Isaac Gordezky
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CUSTOMMODULE_HPP_
#define CUSTOMMODULE_HPP_

#include <QtDeclarative>
#include <QObject>

namespace custom {
    class Module1: public QObject {
        Q_OBJECT

        Q_PROPERTY (QString constantProperty READ constantProperty CONSTANT)
        Q_PROPERTY (QString fullProperty READ fullProperty WRITE setFullProperty NOTIFY fullPropertyChanged)

    public:
        Module1(QObject *parent = NULL);
        virtual ~Module1();

        Q_SLOT void doSomething();
        Q_INVOKABLE QString doSomethingWithResult();

        QString constantProperty() const;
        QString fullProperty() const;
        void setFullProperty(QString fullProperty);

    Q_SIGNALS:
        void fullPropertyChanged();
        void tick(qulonglong count);

    private:
        Q_SLOT void tick();
        QString mConstantProperty, mFullProperty;
    };
}

QML_DECLARE_TYPE(custom::Module1)

#endif /* CUSTOMMODULE_HPP_ */
