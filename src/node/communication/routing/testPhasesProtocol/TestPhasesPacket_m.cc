//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/testPhasesProtocol/TestPhasesPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "TestPhasesPacket_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

Neighbor::Neighbor()
{
    id = 0;
    rssi = 0;
    residual = 0;
    location = 0;
}

void doPacking(cCommBuffer *b, Neighbor& a)
{
    doPacking(b,a.id);
    doPacking(b,a.rssi);
    doPacking(b,a.residual);
    doPacking(b,a.location);
}

void doUnpacking(cCommBuffer *b, Neighbor& a)
{
    doUnpacking(b,a.id);
    doUnpacking(b,a.rssi);
    doUnpacking(b,a.residual);
    doUnpacking(b,a.location);
}

class NeighborDescriptor : public cClassDescriptor
{
  public:
    NeighborDescriptor();
    virtual ~NeighborDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(NeighborDescriptor);

NeighborDescriptor::NeighborDescriptor() : cClassDescriptor("Neighbor", "")
{
}

NeighborDescriptor::~NeighborDescriptor()
{
}

bool NeighborDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Neighbor *>(obj)!=NULL;
}

const char *NeighborDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int NeighborDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int NeighborDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *NeighborDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "id",
        "rssi",
        "residual",
        "location",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int NeighborDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='i' && strcmp(fieldName, "id")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "rssi")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "residual")==0) return base+2;
    if (fieldName[0]=='l' && strcmp(fieldName, "location")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *NeighborDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "double",
        "double",
        "double",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *NeighborDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int NeighborDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Neighbor *pp = (Neighbor *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string NeighborDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Neighbor *pp = (Neighbor *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->id);
        case 1: return double2string(pp->rssi);
        case 2: return double2string(pp->residual);
        case 3: return double2string(pp->location);
        default: return "";
    }
}

bool NeighborDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Neighbor *pp = (Neighbor *)object; (void)pp;
    switch (field) {
        case 0: pp->id = string2long(value); return true;
        case 1: pp->rssi = string2double(value); return true;
        case 2: pp->residual = string2double(value); return true;
        case 3: pp->location = string2double(value); return true;
        default: return false;
    }
}

const char *NeighborDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *NeighborDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Neighbor *pp = (Neighbor *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

tdma::tdma()
{
    id = 0;
    weight = 0;
}

void doPacking(cCommBuffer *b, tdma& a)
{
    doPacking(b,a.id);
    doPacking(b,a.weight);
}

void doUnpacking(cCommBuffer *b, tdma& a)
{
    doUnpacking(b,a.id);
    doUnpacking(b,a.weight);
}

class tdmaDescriptor : public cClassDescriptor
{
  public:
    tdmaDescriptor();
    virtual ~tdmaDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(tdmaDescriptor);

tdmaDescriptor::tdmaDescriptor() : cClassDescriptor("tdma", "")
{
}

tdmaDescriptor::~tdmaDescriptor()
{
}

bool tdmaDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<tdma *>(obj)!=NULL;
}

const char *tdmaDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int tdmaDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int tdmaDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *tdmaDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "id",
        "weight",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int tdmaDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='i' && strcmp(fieldName, "id")==0) return base+0;
    if (fieldName[0]=='w' && strcmp(fieldName, "weight")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *tdmaDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *tdmaDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int tdmaDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    tdma *pp = (tdma *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string tdmaDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    tdma *pp = (tdma *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->id);
        case 1: return long2string(pp->weight);
        default: return "";
    }
}

bool tdmaDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    tdma *pp = (tdma *)object; (void)pp;
    switch (field) {
        case 0: pp->id = string2long(value); return true;
        case 1: pp->weight = string2long(value); return true;
        default: return false;
    }
}

const char *tdmaDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *tdmaDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    tdma *pp = (tdma *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("TestPhasesPacket_Type");
    if (!e) enums.getInstance()->add(e = new cEnum("TestPhasesPacket_Type"));
    e->insert(HELLO_WORD_PACKET, "HELLO_WORD_PACKET");
    e->insert(BROADCAST_PACKET, "BROADCAST_PACKET");
    e->insert(TO_SINK_PACKET, "TO_SINK_PACKET");
    e->insert(SENT_TO_SINK_PACKET, "SENT_TO_SINK_PACKET");
    e->insert(END_SETUP_PACKET, "END_SETUP_PACKET");
    e->insert(JOIN_CH_PACKET, "JOIN_CH_PACKET");
    e->insert(ACK_CH_PACKET, "ACK_CH_PACKET");
    e->insert(CLUSTER_MEMBER_PACKET, "CLUSTER_MEMBER_PACKET");
    e->insert(NODE_ACK_CH_PACKET, "NODE_ACK_CH_PACKET");
    e->insert(TDMA_CH_PACKET, "TDMA_CH_PACKET");
    e->insert(DATA_CH_PACKET, "DATA_CH_PACKET");
);

Register_Class(TestPhasesPacket);

TestPhasesPacket::TestPhasesPacket(const char *name, int kind) : ::RoutingPacket(name,kind)
{
    this->hopNumb_var = 0;
    this->currentRound_var = 0;
    this->sinkRssi_var = 0;
    this->location_var = 0;
    this->sumEnergy_var = 0;
    this->residual_var = 0;
    this->moisture_var = 0;
    this->followerNumb_var = 0;
    this->TestPhasesPacketKind_var = 0;
    schedule_arraysize = 0;
    this->schedule_var = 0;
    Neighbors_arraysize = 0;
    this->Neighbors_var = 0;
}

TestPhasesPacket::TestPhasesPacket(const TestPhasesPacket& other) : ::RoutingPacket(other)
{
    schedule_arraysize = 0;
    this->schedule_var = 0;
    Neighbors_arraysize = 0;
    this->Neighbors_var = 0;
    copy(other);
}

TestPhasesPacket::~TestPhasesPacket()
{
    delete [] schedule_var;
    delete [] Neighbors_var;
}

TestPhasesPacket& TestPhasesPacket::operator=(const TestPhasesPacket& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void TestPhasesPacket::copy(const TestPhasesPacket& other)
{
    this->hopNumb_var = other.hopNumb_var;
    this->currentRound_var = other.currentRound_var;
    this->sinkRssi_var = other.sinkRssi_var;
    this->location_var = other.location_var;
    this->sumEnergy_var = other.sumEnergy_var;
    this->residual_var = other.residual_var;
    this->moisture_var = other.moisture_var;
    this->followerNumb_var = other.followerNumb_var;
    this->TestPhasesPacketKind_var = other.TestPhasesPacketKind_var;
    delete [] this->schedule_var;
    this->schedule_var = (other.schedule_arraysize==0) ? NULL : new int[other.schedule_arraysize];
    schedule_arraysize = other.schedule_arraysize;
    for (unsigned int i=0; i<schedule_arraysize; i++)
        this->schedule_var[i] = other.schedule_var[i];
    delete [] this->Neighbors_var;
    this->Neighbors_var = (other.Neighbors_arraysize==0) ? NULL : new Neighbor[other.Neighbors_arraysize];
    Neighbors_arraysize = other.Neighbors_arraysize;
    for (unsigned int i=0; i<Neighbors_arraysize; i++)
        this->Neighbors_var[i] = other.Neighbors_var[i];
}

void TestPhasesPacket::parsimPack(cCommBuffer *b)
{
    ::RoutingPacket::parsimPack(b);
    doPacking(b,this->hopNumb_var);
    doPacking(b,this->currentRound_var);
    doPacking(b,this->sinkRssi_var);
    doPacking(b,this->location_var);
    doPacking(b,this->sumEnergy_var);
    doPacking(b,this->residual_var);
    doPacking(b,this->moisture_var);
    doPacking(b,this->followerNumb_var);
    doPacking(b,this->TestPhasesPacketKind_var);
    b->pack(schedule_arraysize);
    doPacking(b,this->schedule_var,schedule_arraysize);
    b->pack(Neighbors_arraysize);
    doPacking(b,this->Neighbors_var,Neighbors_arraysize);
}

void TestPhasesPacket::parsimUnpack(cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->hopNumb_var);
    doUnpacking(b,this->currentRound_var);
    doUnpacking(b,this->sinkRssi_var);
    doUnpacking(b,this->location_var);
    doUnpacking(b,this->sumEnergy_var);
    doUnpacking(b,this->residual_var);
    doUnpacking(b,this->moisture_var);
    doUnpacking(b,this->followerNumb_var);
    doUnpacking(b,this->TestPhasesPacketKind_var);
    delete [] this->schedule_var;
    b->unpack(schedule_arraysize);
    if (schedule_arraysize==0) {
        this->schedule_var = 0;
    } else {
        this->schedule_var = new int[schedule_arraysize];
        doUnpacking(b,this->schedule_var,schedule_arraysize);
    }
    delete [] this->Neighbors_var;
    b->unpack(Neighbors_arraysize);
    if (Neighbors_arraysize==0) {
        this->Neighbors_var = 0;
    } else {
        this->Neighbors_var = new Neighbor[Neighbors_arraysize];
        doUnpacking(b,this->Neighbors_var,Neighbors_arraysize);
    }
}

int TestPhasesPacket::getHopNumb() const
{
    return hopNumb_var;
}

void TestPhasesPacket::setHopNumb(int hopNumb)
{
    this->hopNumb_var = hopNumb;
}

int TestPhasesPacket::getCurrentRound() const
{
    return currentRound_var;
}

void TestPhasesPacket::setCurrentRound(int currentRound)
{
    this->currentRound_var = currentRound;
}

double TestPhasesPacket::getSinkRssi() const
{
    return sinkRssi_var;
}

void TestPhasesPacket::setSinkRssi(double sinkRssi)
{
    this->sinkRssi_var = sinkRssi;
}

double TestPhasesPacket::getLocation() const
{
    return location_var;
}

void TestPhasesPacket::setLocation(double location)
{
    this->location_var = location;
}

double TestPhasesPacket::getSumEnergy() const
{
    return sumEnergy_var;
}

void TestPhasesPacket::setSumEnergy(double sumEnergy)
{
    this->sumEnergy_var = sumEnergy;
}

double TestPhasesPacket::getResidual() const
{
    return residual_var;
}

void TestPhasesPacket::setResidual(double residual)
{
    this->residual_var = residual;
}

double TestPhasesPacket::getMoisture() const
{
    return moisture_var;
}

void TestPhasesPacket::setMoisture(double moisture)
{
    this->moisture_var = moisture;
}

int TestPhasesPacket::getFollowerNumb() const
{
    return followerNumb_var;
}

void TestPhasesPacket::setFollowerNumb(int followerNumb)
{
    this->followerNumb_var = followerNumb;
}

int TestPhasesPacket::getTestPhasesPacketKind() const
{
    return TestPhasesPacketKind_var;
}

void TestPhasesPacket::setTestPhasesPacketKind(int TestPhasesPacketKind)
{
    this->TestPhasesPacketKind_var = TestPhasesPacketKind;
}

void TestPhasesPacket::setScheduleArraySize(unsigned int size)
{
    int *schedule_var2 = (size==0) ? NULL : new int[size];
    unsigned int sz = schedule_arraysize < size ? schedule_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        schedule_var2[i] = this->schedule_var[i];
    for (unsigned int i=sz; i<size; i++)
        schedule_var2[i] = 0;
    schedule_arraysize = size;
    delete [] this->schedule_var;
    this->schedule_var = schedule_var2;
}

unsigned int TestPhasesPacket::getScheduleArraySize() const
{
    return schedule_arraysize;
}

int TestPhasesPacket::getSchedule(unsigned int k) const
{
    if (k>=schedule_arraysize) throw cRuntimeError("Array of size %d indexed by %d", schedule_arraysize, k);
    return schedule_var[k];
}

void TestPhasesPacket::setSchedule(unsigned int k, int schedule)
{
    if (k>=schedule_arraysize) throw cRuntimeError("Array of size %d indexed by %d", schedule_arraysize, k);
    this->schedule_var[k] = schedule;
}

void TestPhasesPacket::setNeighborsArraySize(unsigned int size)
{
    Neighbor *Neighbors_var2 = (size==0) ? NULL : new Neighbor[size];
    unsigned int sz = Neighbors_arraysize < size ? Neighbors_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        Neighbors_var2[i] = this->Neighbors_var[i];
    Neighbors_arraysize = size;
    delete [] this->Neighbors_var;
    this->Neighbors_var = Neighbors_var2;
}

unsigned int TestPhasesPacket::getNeighborsArraySize() const
{
    return Neighbors_arraysize;
}

Neighbor& TestPhasesPacket::getNeighbors(unsigned int k)
{
    if (k>=Neighbors_arraysize) throw cRuntimeError("Array of size %d indexed by %d", Neighbors_arraysize, k);
    return Neighbors_var[k];
}

void TestPhasesPacket::setNeighbors(unsigned int k, const Neighbor& Neighbors)
{
    if (k>=Neighbors_arraysize) throw cRuntimeError("Array of size %d indexed by %d", Neighbors_arraysize, k);
    this->Neighbors_var[k] = Neighbors;
}

class TestPhasesPacketDescriptor : public cClassDescriptor
{
  public:
    TestPhasesPacketDescriptor();
    virtual ~TestPhasesPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(TestPhasesPacketDescriptor);

TestPhasesPacketDescriptor::TestPhasesPacketDescriptor() : cClassDescriptor("TestPhasesPacket", "RoutingPacket")
{
}

TestPhasesPacketDescriptor::~TestPhasesPacketDescriptor()
{
}

bool TestPhasesPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<TestPhasesPacket *>(obj)!=NULL;
}

const char *TestPhasesPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int TestPhasesPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 11+basedesc->getFieldCount(object) : 11;
}

unsigned int TestPhasesPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISCOMPOUND,
    };
    return (field>=0 && field<11) ? fieldTypeFlags[field] : 0;
}

const char *TestPhasesPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "hopNumb",
        "currentRound",
        "sinkRssi",
        "location",
        "sumEnergy",
        "residual",
        "moisture",
        "followerNumb",
        "TestPhasesPacketKind",
        "schedule",
        "Neighbors",
    };
    return (field>=0 && field<11) ? fieldNames[field] : NULL;
}

int TestPhasesPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopNumb")==0) return base+0;
    if (fieldName[0]=='c' && strcmp(fieldName, "currentRound")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "sinkRssi")==0) return base+2;
    if (fieldName[0]=='l' && strcmp(fieldName, "location")==0) return base+3;
    if (fieldName[0]=='s' && strcmp(fieldName, "sumEnergy")==0) return base+4;
    if (fieldName[0]=='r' && strcmp(fieldName, "residual")==0) return base+5;
    if (fieldName[0]=='m' && strcmp(fieldName, "moisture")==0) return base+6;
    if (fieldName[0]=='f' && strcmp(fieldName, "followerNumb")==0) return base+7;
    if (fieldName[0]=='T' && strcmp(fieldName, "TestPhasesPacketKind")==0) return base+8;
    if (fieldName[0]=='s' && strcmp(fieldName, "schedule")==0) return base+9;
    if (fieldName[0]=='N' && strcmp(fieldName, "Neighbors")==0) return base+10;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *TestPhasesPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "double",
        "double",
        "double",
        "double",
        "double",
        "int",
        "int",
        "int",
        "Neighbor",
    };
    return (field>=0 && field<11) ? fieldTypeStrings[field] : NULL;
}

const char *TestPhasesPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 8:
            if (!strcmp(propertyname,"enum")) return "TestPhasesPacket_Type";
            return NULL;
        default: return NULL;
    }
}

int TestPhasesPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    TestPhasesPacket *pp = (TestPhasesPacket *)object; (void)pp;
    switch (field) {
        case 9: return pp->getScheduleArraySize();
        case 10: return pp->getNeighborsArraySize();
        default: return 0;
    }
}

std::string TestPhasesPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    TestPhasesPacket *pp = (TestPhasesPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getHopNumb());
        case 1: return long2string(pp->getCurrentRound());
        case 2: return double2string(pp->getSinkRssi());
        case 3: return double2string(pp->getLocation());
        case 4: return double2string(pp->getSumEnergy());
        case 5: return double2string(pp->getResidual());
        case 6: return double2string(pp->getMoisture());
        case 7: return long2string(pp->getFollowerNumb());
        case 8: return long2string(pp->getTestPhasesPacketKind());
        case 9: return long2string(pp->getSchedule(i));
        case 10: {std::stringstream out; out << pp->getNeighbors(i); return out.str();}
        default: return "";
    }
}

bool TestPhasesPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    TestPhasesPacket *pp = (TestPhasesPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setHopNumb(string2long(value)); return true;
        case 1: pp->setCurrentRound(string2long(value)); return true;
        case 2: pp->setSinkRssi(string2double(value)); return true;
        case 3: pp->setLocation(string2double(value)); return true;
        case 4: pp->setSumEnergy(string2double(value)); return true;
        case 5: pp->setResidual(string2double(value)); return true;
        case 6: pp->setMoisture(string2double(value)); return true;
        case 7: pp->setFollowerNumb(string2long(value)); return true;
        case 8: pp->setTestPhasesPacketKind(string2long(value)); return true;
        case 9: pp->setSchedule(i,string2long(value)); return true;
        default: return false;
    }
}

const char *TestPhasesPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 10: return opp_typename(typeid(Neighbor));
        default: return NULL;
    };
}

void *TestPhasesPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    TestPhasesPacket *pp = (TestPhasesPacket *)object; (void)pp;
    switch (field) {
        case 10: return (void *)(&pp->getNeighbors(i)); break;
        default: return NULL;
    }
}


