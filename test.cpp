#ifndef __PROGTEST__

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>
#include "ipaddress.h"

using namespace std;
#endif /* __PROGTEST__ */

class ClassIP {
protected:
    string m_Name;
    string m_Type;
public:

    ClassIP(void){}
    virtual ~ClassIP(void) = default;
    virtual string Name() const = 0;

    virtual string Type() const = 0;

    virtual ostream &print(ostream &os, const ClassIP &CIP) const {
        os << "+-" << m_Name << m_Type;
        return os;
    }


};


class CRecA : public ClassIP {
public:
    // constructor

    CRecA(const string &name, const CIPv4 &CIP4) :ClassIP(){
        m_Name = name;
    }


    // Name ()
    string Name() const override {
        return ClassIP::m_Name;
    }

    // Type ()
    string Type() const override {
        return ClassIP::m_Type;
    }

    // operator <<
    // todo
};

class CRecAAAA :public ClassIP {
public:
    // constructor
    CRecAAAA(const string &name, const CIPv6 &CIP6) : ClassIP(){
        m_Name = name;

    }


    // Name ()
    string Name() const override {
        return ClassIP::m_Name;
    }

    // Type ()
    string Type() const override {
        return ClassIP::m_Type;
    }
    // operator <<

    // todo
};

class CRecMX : public ClassIP {
    int priority;
    string domain;
public:
    // constructor

    CRecMX(const string &name, const string &gDomain, const int &gPriority) : ClassIP() {
        domain = gDomain;
        m_Name = name;
        priority = gPriority;
    }

    // Name ()
    string Name() const override {
        return ClassIP::m_Name;
    }

    // Type ()
    string Type() const override {
        return ClassIP::m_Type;
    }
    // operator <<
    // todo
};

class CZone {
    vector<shared_ptr<ClassIP>> records;
    string m_Name;
public:

    CZone(const string &mName) : m_Name(mName) {}


    // destructor (if needed)
    // operator = (if needed)
    // Add ()


    bool Add(const ClassIP & record) {

        return false;
    }


    // Del ()
    // Search ()
    CZone & Search(const string & name){
        CZone zone;


        return zone;
    }

    // operator <<
    friend operator << (const  CZone & z){
        
    }


    // todo
};

#ifndef __PROGTEST__

int main(void) {
    ostringstream oss;

    CZone z0("fit.cvut.cz");
    assert (z0.Add(CRecA("progtest", CIPv4("147.32.232.142"))) == true);
    assert (z0.Add(CRecAAAA("progtest", CIPv6("2001:718:2:2902:0:1:2:3"))) == true);
    assert (z0.Add(CRecA("courses", CIPv4("147.32.232.158"))) == true);
    assert (z0.Add(CRecA("courses", CIPv4("147.32.232.160"))) == true);
    assert (z0.Add(CRecA("courses", CIPv4("147.32.232.159"))) == true);
    assert (z0.Add(CRecAAAA("progtest", CIPv6("2001:718:2:2902:1:2:3:4"))) == true);
    assert (z0.Add(CRecMX("courses", "relay.fit.cvut.cz.", 0)) == true);
    assert (z0.Add(CRecMX("courses", "relay2.fit.cvut.cz.", 10)) == true);
    oss.str("");
    oss << z0;
    assert (oss.str() ==
            "fit.cvut.cz\n"
            " +- progtest A 147.32.232.142\n"
            " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
            " +- courses A 147.32.232.158\n"
            " +- courses A 147.32.232.160\n"
            " +- courses A 147.32.232.159\n"
            " +- progtest AAAA 2001:718:2:2902:1:2:3:4\n"
            " +- courses MX 0 relay.fit.cvut.cz.\n"
            " \\- courses MX 10 relay2.fit.cvut.cz.\n");
    assert (z0.Search("progtest").Count() == 3);
    oss.str("");
    oss << z0.Search("progtest");
    assert (oss.str() ==
            "progtest A 147.32.232.142\n"
            "progtest AAAA 2001:718:2:2902:0:1:2:3\n"
            "progtest AAAA 2001:718:2:2902:1:2:3:4\n");
    assert (z0.Del(CRecA("courses", CIPv4("147.32.232.160"))) == true);
    assert (z0.Add(CRecA("courses", CIPv4("147.32.232.122"))) == true);
    oss.str("");
    oss << z0;
    assert (oss.str() ==
            "fit.cvut.cz\n"
            " +- progtest A 147.32.232.142\n"
            " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
            " +- courses A 147.32.232.158\n"
            " +- courses A 147.32.232.159\n"
            " +- progtest AAAA 2001:718:2:2902:1:2:3:4\n"
            " +- courses MX 0 relay.fit.cvut.cz.\n"
            " +- courses MX 10 relay2.fit.cvut.cz.\n"
            " \\- courses A 147.32.232.122\n");
    assert (z0.Search("courses").Count() == 5);
    oss.str("");
    oss << z0.Search("courses");
    assert (oss.str() ==
            "courses A 147.32.232.158\n"
            "courses A 147.32.232.159\n"
            "courses MX 0 relay.fit.cvut.cz.\n"
            "courses MX 10 relay2.fit.cvut.cz.\n"
            "courses A 147.32.232.122\n");
    oss.str("");
    oss << z0.Search("courses")[0];
    assert (oss.str() == "courses A 147.32.232.158");
    assert (z0.Search("courses")[0].Name() == "courses");
    assert (z0.Search("courses")[0].Type() == "A");
    oss.str("");
    oss << z0.Search("courses")[1];
    assert (oss.str() == "courses A 147.32.232.159");
    assert (z0.Search("courses")[1].Name() == "courses");
    assert (z0.Search("courses")[1].Type() == "A");
    oss.str("");
    oss << z0.Search("courses")[2];
    assert (oss.str() == "courses MX 0 relay.fit.cvut.cz.");
    assert (z0.Search("courses")[2].Name() == "courses");
    assert (z0.Search("courses")[2].Type() == "MX");
    try {
        oss.str("");
        oss << z0.Search("courses")[10];
        assert ("No exception thrown!" == nullptr);
    }
    catch (const out_of_range &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }
    dynamic_cast<const CRecAAAA &> ( z0.Search("progtest")[1] );
    CZone z1("fit2.cvut.cz");
    z1.Add(z0.Search("progtest")[2]);
    z1.Add(z0.Search("progtest")[0]);
    z1.Add(z0.Search("progtest")[1]);
    z1.Add(z0.Search("courses")[2]);
    oss.str("");
    oss << z1;
    assert (oss.str() ==
            "fit2.cvut.cz\n"
            " +- progtest AAAA 2001:718:2:2902:1:2:3:4\n"
            " +- progtest A 147.32.232.142\n"
            " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
            " \\- courses MX 0 relay.fit.cvut.cz.\n");
    dynamic_cast<const CRecA &> ( z1.Search("progtest")[1] );

    CZone z2("fit.cvut.cz");
    assert (z2.Add(CRecA("progtest", CIPv4("147.32.232.142"))) == true);
    assert (z2.Add(CRecAAAA("progtest", CIPv6("2001:718:2:2902:0:1:2:3"))) == true);
    assert (z2.Add(CRecA("progtest", CIPv4("147.32.232.144"))) == true);
    assert (z2.Add(CRecMX("progtest", "relay.fit.cvut.cz.", 10)) == true);
    assert (z2.Add(CRecA("progtest", CIPv4("147.32.232.142"))) == false);
    assert (z2.Del(CRecA("progtest", CIPv4("147.32.232.140"))) == false);
    assert (z2.Del(CRecA("progtest", CIPv4("147.32.232.142"))) == true);
    assert (z2.Del(CRecA("progtest", CIPv4("147.32.232.142"))) == false);
    assert (z2.Add(CRecMX("progtest", "relay.fit.cvut.cz.", 20)) == true);
    assert (z2.Add(CRecMX("progtest", "relay.fit.cvut.cz.", 10)) == false);
    oss.str("");
    oss << z2;
    assert (oss.str() ==
            "fit.cvut.cz\n"
            " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
            " +- progtest A 147.32.232.144\n"
            " +- progtest MX 10 relay.fit.cvut.cz.\n"
            " \\- progtest MX 20 relay.fit.cvut.cz.\n");
    assert (z2.Search("progtest").Count() == 4);
    oss.str("");
    oss << z2.Search("progtest");
    assert (oss.str() ==
            "progtest AAAA 2001:718:2:2902:0:1:2:3\n"
            "progtest A 147.32.232.144\n"
            "progtest MX 10 relay.fit.cvut.cz.\n"
            "progtest MX 20 relay.fit.cvut.cz.\n");
    assert (z2.Search("courses").Count() == 0);
    oss.str("");
    oss << z2.Search("courses");
    assert (oss.str() == "");
    try {
        dynamic_cast<const CRecMX &> ( z2.Search("progtest")[0] );
        assert ("Invalid type" == nullptr);
    }
    catch (const bad_cast &e) {
    }

    CZone z4("fit.cvut.cz");
    assert (z4.Add(CRecA("progtest", CIPv4("147.32.232.142"))) == true);
    assert (z4.Add(CRecA("courses", CIPv4("147.32.232.158"))) == true);
    assert (z4.Add(CRecA("courses", CIPv4("147.32.232.160"))) == true);
    assert (z4.Add(CRecA("courses", CIPv4("147.32.232.159"))) == true);
    CZone z5(z4);
    assert (z4.Add(CRecAAAA("progtest", CIPv6("2001:718:2:2902:0:1:2:3"))) == true);
    assert (z4.Add(CRecAAAA("progtest", CIPv6("2001:718:2:2902:1:2:3:4"))) == true);
    assert (z5.Del(CRecA("progtest", CIPv4("147.32.232.142"))) == true);
    oss.str("");
    oss << z4;
    assert (oss.str() ==
            "fit.cvut.cz\n"
            " +- progtest A 147.32.232.142\n"
            " +- courses A 147.32.232.158\n"
            " +- courses A 147.32.232.160\n"
            " +- courses A 147.32.232.159\n"
            " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
            " \\- progtest AAAA 2001:718:2:2902:1:2:3:4\n");
    oss.str("");
    oss << z5;
    assert (oss.str() ==
            "fit.cvut.cz\n"
            " +- courses A 147.32.232.158\n"
            " +- courses A 147.32.232.160\n"
            " \\- courses A 147.32.232.159\n");
    z5 = z4;
    assert (z4.Add(CRecMX("courses", "relay.fit.cvut.cz.", 0)) == true);
    assert (z4.Add(CRecMX("courses", "relay2.fit.cvut.cz.", 10)) == true);
    oss.str("");
    oss << z4;
    assert (oss.str() ==
            "fit.cvut.cz\n"
            " +- progtest A 147.32.232.142\n"
            " +- courses A 147.32.232.158\n"
            " +- courses A 147.32.232.160\n"
            " +- courses A 147.32.232.159\n"
            " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
            " +- progtest AAAA 2001:718:2:2902:1:2:3:4\n"
            " +- courses MX 0 relay.fit.cvut.cz.\n"
            " \\- courses MX 10 relay2.fit.cvut.cz.\n");
    oss.str("");
    oss << z5;
    assert (oss.str() ==
            "fit.cvut.cz\n"
            " +- progtest A 147.32.232.142\n"
            " +- courses A 147.32.232.158\n"
            " +- courses A 147.32.232.160\n"
            " +- courses A 147.32.232.159\n"
            " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
            " \\- progtest AAAA 2001:718:2:2902:1:2:3:4\n");

    return 0;
}

#endif /* __PROGTEST__ */
