/*
* This file is part of the API Extractor project.
*
* Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
*
* Contact: PySide team <contact@pyside.org>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* version 2 as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
* 02110-1301 USA
*
*/

#include "testconversionoperator.h"
#include <QtTest/QTest>
#include "testutil.h"


void TestConversionOperator::testConversionOperator()
{
    const char cppCode[] = "\
    struct A {\
    };\
    struct B {\
        operator A() const;\
    };\
    struct C {\
        operator A() const;\
    };";
    const char xmlCode[] = "\
    <typesystem package=\"Foo\">\
        <value-type name='A' />\
        <value-type name='B' />\
        <value-type name='C' />\
    </typesystem>";

    TestUtil t(cppCode, xmlCode);
    AbstractMetaClassList classes = t.builder()->classes();
    AbstractMetaClass* classA = classes.findClass("A");
    AbstractMetaClass* classB = classes.findClass("B");
    AbstractMetaClass* classC = classes.findClass("C");
    QVERIFY(classA);
    QVERIFY(classB);
    QVERIFY(classC);
    QCOMPARE(classA->functions().count(), 1);
    QCOMPARE(classB->functions().count(), 2);
    QCOMPARE(classC->functions().count(), 2);
    QCOMPARE(classA->externalConversionOperators().count(), 2);

    const AbstractMetaFunction* convOp = 0;
    foreach(const AbstractMetaFunction* func, classB->functions()) {
        if (func->isConversionOperator())
            convOp = func;
    }
    QVERIFY(convOp);
    const AbstractMetaFunction* externalConvOp = classA->externalConversionOperators().first();
    QCOMPARE(convOp, externalConvOp);
}

void TestConversionOperator::testConversionOperatorOfDiscardedClass()
{
    const char cppCode[] = "\
    struct A {\
    };\
    struct B {\
        operator A() const;\
    };";
    const char xmlCode[] = "\
    <typesystem package=\"Foo\">\
        <value-type name='A' />\
    </typesystem>";

    TestUtil t(cppCode, xmlCode);
    AbstractMetaClassList classes = t.builder()->classes();
    AbstractMetaClass* classA = classes.findClass("A");
    QVERIFY(classA);
    QCOMPARE(classA->externalConversionOperators().count(), 0);
}

void TestConversionOperator::testRemovedConversionOperator()
{
    const char cppCode[] = "\
    struct A {\
    };\
    struct B {\
        operator A() const;\
    };";
    const char xmlCode[] = "\
    <typesystem package=\"Foo\">\
        <value-type name='A' />\
        <value-type name='B'>\
            <modify-function signature='operator A() const' remove='all' />\
        </value-type>\
    </typesystem>";

    TestUtil t(cppCode, xmlCode);
    AbstractMetaClassList classes = t.builder()->classes();
    AbstractMetaClass* classA = classes.findClass("A");
    AbstractMetaClass* classB = classes.findClass("B");
    QVERIFY(classA);
    QVERIFY(classB);
    QCOMPARE(classA->functions().count(), 1);
    QCOMPARE(classB->functions().count(), 2);
    QCOMPARE(classA->externalConversionOperators().count(), 0);
    QCOMPARE(classA->implicitConversions().count(), 0);
}

QTEST_APPLESS_MAIN(TestConversionOperator)

#include "testconversionoperator.moc"