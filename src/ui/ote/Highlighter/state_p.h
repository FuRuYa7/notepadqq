/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>
    Copyright (C) 2018 Christoph Cullmann <cullmann@kde.org>

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef KSYNTAXHIGHLIGHTING_STATE_P_H
#define KSYNTAXHIGHLIGHTING_STATE_P_H

#include <QSharedData>
#include <QVector>

#include "definitionref_p.h"

QT_BEGIN_NAMESPACE
class QStringList;
QT_END_NAMESPACE

namespace ote
{

class Context;

class StateData : public QSharedData
{
    friend class State;
    friend class AbstractHighlighter;

public:
    StateData() = default;
    static StateData* get(State &state);

    bool isEmpty() const;
    void clear();
    int size() const;
    void push(Context *context, const QStringList &captures);
    void pop();
    Context* topContext() const;
    const QStringList &topCaptures() const;

private:
    /**
     * weak reference to the used definition to filter out invalid states
     */
    DefinitionRef m_defRef;

    /**
     * the context stack combines the active context + valid captures
     */
    QVector<QPair<Context *, QStringList>> m_contextStack;
};

}

#endif
