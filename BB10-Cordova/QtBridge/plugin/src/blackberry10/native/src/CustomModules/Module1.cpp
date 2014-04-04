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

#include <CustomModules/Module1.hpp>

#include <QTimer>

namespace custom
{

    Module1::Module1(QObject *parent) :
            QObject(parent), mConstantProperty("constantPropertyValue"), mFullProperty(
                    "mFullPropertyValue")
    {
        // TODO Auto-generated constructor stub
        QTimer * timer = new QTimer(this);
        timer->setInterval(2500);
        timer->setSingleShot(false);
        timer->start();
        connect(timer, SIGNAL(timeout()), SLOT(tick()));
    }

    Module1::~Module1()
    {
        // TODO Auto-generated destructor stub
    }

    void Module1::tick() {
        static qulonglong count = 0;
        Q_EMIT (tick(count++));
    }

    void Module1::doSomething() {
        // TODO Add custom code here
    }

    QString Module1::doSomethingWithResult() {
        // TODO Add custom code here
        return "done";
    }

    QString Module1::constantProperty() const
    {
        return mConstantProperty;
    }

    QString Module1::fullProperty() const
    {
        return mFullProperty;
    }
    void Module1::setFullProperty(QString fullProperty)
    {
        mFullProperty = fullProperty;
        Q_EMIT fullPropertyChanged();
    }

} /* end namespace custom */
