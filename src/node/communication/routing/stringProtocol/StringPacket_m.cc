//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/stringProtocol/StringPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "StringPacket_m.h"

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

stringNeighbor::stringNeighbor()
{
    id = 0;
    kVal = 0;
    rssi = 0;
    residual = 0;
    location = 0;
}

void doPacking(cCommBuffer *b, stringNeighbor& a)
{
    doPacking(b,a.id);
    doPacking(b,a.kVal);
    doPacking(b,a.rssi);
    doPacking(b,a.residual);
    doPacking(b,a.location);
}

void doUnpacking(cCommBuffer *b, stringNeighbor& a)
{
    doUnpacking(b,a.id);
    doUnpacking(b,a.kVal);
    doUnpacking(b,a.rssi);
    doUnpacking(b,a.residual);
    doUnpacking(b,a.location);
}

class stringNeighborDescriptor : public cClassDescriptor
{
  public:
    stringNeighborDescriptor();
    virtual ~stringNeighborDescriptor();

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

Register_ClassDescriptor(stringNeighborDescriptor);

stringNeighborDescriptor::stringNeighborDescriptor() : cClassDescriptor("stringNeighbor", "")
{
}

stringNeighborDescriptor::~stringNeighborDescriptor()
{
}

bool stringNeighborDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<stringNeighbor *>(obj)!=NULL;
}

const char *stringNeighborDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int stringNeighborDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount(object) : 5;
}

unsigned int stringNeighborDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *stringNeighborDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "id",
        "kVal",
        "rssi",
        "residual",
        "location",
    };
    return (field>=0 && field<5) ? fieldNames[field] : NULL;
}

int stringNeighborDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='i' && strcmp(fieldName, "id")==0) return base+0;
    if (fieldName[0]=='k' && strcmp(fieldName, "kVal")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "rssi")==0) return base+2;
    if (fieldName[0]=='r' && strcmp(fieldName, "residual")==0) return base+3;
    if (fieldName[0]=='l' && strcmp(fieldName, "location")==0) return base+4;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *stringNeighborDescriptor::getFieldTypeString(void *object, int field) const
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
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : NULL;
}

const char *stringNeighborDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int stringNeighborDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    stringNeighbor *pp = (stringNeighbor *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string stringNeighborDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    stringNeighbor *pp = (stringNeighbor *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->id);
        case 1: return long2string(pp->kVal);
        case 2: return double2string(pp->rssi);
        case 3: return double2string(pp->residual);
        case 4: return double2string(pp->location);
        default: return "";
    }
}

bool stringNeighborDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    stringNeighbor *pp = (stringNeighbor *)object; (void)pp;
    switch (field) {
        case 0: pp->id = string2long(value); return true;
        case 1: pp->kVal = string2long(value); return true;
        case 2: pp->rssi = string2double(value); return true;
        case 3: pp->residual = string2double(value); return true;
        case 4: pp->location = string2double(value); return true;
        default: return false;
    }
}

const char *stringNeighborDescriptor::getFieldStructName(void *object, int field) const
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

void *stringNeighborDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    stringNeighbor *pp = (stringNeighbor *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("StringPacket_Type");
    if (!e) enums.getInstance()->add(e = new cEnum("StringPacket_Type"));
    e->insert(HELLO_WORD_PACKET, "HELLO_WORD_PACKET");
    e->insert(BROADCAST_PACKET, "BROADCAST_PACKET");
    e->insert(TO_SINK_PACKET, "TO_SINK_PACKET");
    e->insert(SENT_TO_SINK_PACKET, "SENT_TO_SINK_PACKET");
    e->insert(END_SETUP_PACKET, "END_SETUP_PACKET");
    e->insert(JOIN_RING_PACKET, "JOIN_RING_PACKET");
    e->insert(ACK_RING_PACKET, "ACK_RING_PACKET");
    e->insert(TDMA_RING_PACKET, "TDMA_RING_PACKET");
    e->insert(DATA_RING_PACKET, "DATA_RING_PACKET");
);

Register_Class(StringPacket);

StringPacket::StringPacket(const char *name, int kind) : ::RoutingPacket(name,kind)
{
    this->Isource_var = 0;
    this->sinkPosition_var = 0;
    this->currentRound_var = 0;
    this->hopNumber_var = 0;
    this->sinkRssi_var = 0;
    this->location_var = 0;
    this->residual_var = 0;
    this->moisture_var = 0;
    this->StringPacketKind_var = 0;
    schedule_arraysize = 0;
    this->schedule_var = 0;
    Neighbors_arraysize = 0;
    this->Neighbors_var = 0;
}

StringPacket::StringPacket(const StringPacket& other) : ::RoutingPacket(other)
{
    schedule_arraysize = 0;
    this->schedule_var = 0;
    Neighbors_arraysize = 0;
    this->Neighbors_var = 0;
    copy(other);
}

StringPacket::~StringPacket()
{
    delete [] schedule_var;
    delete [] Neighbors_var;
}

StringPacket& StringPacket::operator=(const StringPacket& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void StringPacket::copy(const StringPacket& other)
{
    this->Isource_var = other.Isource_var;
    this->sinkPosition_var = other.sinkPosition_var;
    this->currentRound_var = other.currentRound_var;
    this->hopNumber_var = other.hopNumber_var;
    this->sinkRssi_var = other.sinkRssi_var;
    this->location_var = other.location_var;
    this->residual_var = other.residual_var;
    this->moisture_var = other.moisture_var;
    this->StringPacketKind_var = other.StringPacketKind_var;
    delete [] this->schedule_var;
    this->schedule_var = (other.schedule_arraysize==0) ? NULL : new int[other.schedule_arraysize];
    schedule_arraysize = other.schedule_arraysize;
    for (unsigned int i=0; i<schedule_arraysize; i++)
        this->schedule_var[i] = other.schedule_var[i];
    delete [] this->Neighbors_var;
    this->Neighbors_var = (other.Neighbors_arraysize==0) ? NULL : new stringNeighbor[other.Neighbors_arraysize];
    Neighbors_arraysize = other.Neighbors_arraysize;
    for (unsigned int i=0; i<Neighbors_arraysize; i++)
        this->Neighbors_var[i] = other.Neighbors_var[i];
}

void StringPacket::parsimPack(cCommBuffer *b)
{
    ::RoutingPacket::parsimPack(b);
    doPacking(b,this->Isource_var);
    doPacking(b,this->sinkPosition_var);
    doPacking(b,this->currentRound_var);
    doPacking(b,this->hopNumber_var);
    doPacking(b,this->sinkRssi_var);
    doPacking(b,this->location_var);
    doPacking(b,this->residual_var);
    doPacking(b,this->moisture_var);
    doPacking(b,this->StringPacketKind_var);
    b->pack(schedule_arraysize);
    doPacking(b,this->schedule_var,schedule_arraysize);
    b->pack(Neighbors_arraysize);
    doPacking(b,this->Neighbors_var,Neighbors_arraysize);
}

void StringPacket::parsimUnpack(cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->Isource_var);
    doUnpacking(b,this->sinkPosition_var);
    doUnpacking(b,this->currentRound_var);
    doUnpacking(b,this->hopNumber_var);
    doUnpacking(b,this->sinkRssi_var);
    doUnpacking(b,this->location_var);
    doUnpacking(b,this->residual_var);
    doUnpacking(b,this->moisture_var);
    doUnpacking(b,this->StringPacketKind_var);
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
        this->Neighbors_var = new stringNeighbor[Neighbors_arraysize];
        doUnpacking(b,this->Neighbors_var,Neighbors_arraysize);
    }
}

const char * StringPacket::getIsource() const
{
    return Isource_var.c_str();
}

void StringPacket::setIsource(const char * Isource)
{
    this->Isource_var = Isource;
}

int StringPacket::getSinkPosition() const
{
    return sinkPosition_var;
}

void StringPacket::setSinkPosition(int sinkPosition)
{
    this->sinkPosition_var = sinkPosition;
}

int StringPacket::getCurrentRound() const
{
    return currentRound_var;
}

void StringPacket::setCurrentRound(int currentRound)
{
    this->currentRound_var = currentRound;
}

int StringPacket::getHopNumber() const
{
    return hopNumber_var;
}

void StringPacket::setHopNumber(int hopNumber)
{
    this->hopNumber_var = hopNumber;
}

double StringPacket::getSinkRssi() const
{
    return sinkRssi_var;
}

void StringPacket::setSinkRssi(double sinkRssi)
{
    this->sinkRssi_var = sinkRssi;
}

double StringPacket::getLocation() const
{
    return location_var;
}

void StringPacket::setLocation(double location)
{
    this->location_var = location;
}

double StringPacket::getResidual() const
{
    return residual_var;
}

void StringPacket::setResidual(double residual)
{
    this->residual_var = residual;
}

double StringPacket::getMoisture() const
{
    return moisture_var;
}

void StringPacket::setMoisture(double moisture)
{
    this->moisture_var = moisture;
}

int StringPacket::getStringPacketKind() const
{
    return StringPacketKind_var;
}

void StringPacket::setStringPacketKind(int StringPacketKind)
{
    this->StringPacketKind_var = StringPacketKind;
}

void StringPacket::setScheduleArraySize(unsigned int size)
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

unsigned int StringPacket::getScheduleArraySize() const
{
    return schedule_arraysize;
}

int StringPacket::getSchedule(unsigned int k) const
{
    if (k>=schedule_arraysize) throw cRuntimeError("Array of size %d indexed by %d", schedule_arraysize, k);
    return schedule_var[k];
}

void StringPacket::setSchedule(unsigned int k, int schedule)
{
    if (k>=schedule_arraysize) throw cRuntimeError("Array of size %d indexed by %d", schedule_arraysize, k);
    this->schedule_var[k] = schedule;
}

void StringPacket::setNeighborsArraySize(unsigned int size)
{
    stringNeighbor *Neighbors_var2 = (size==0) ? NULL : new stringNeighbor[size];
    unsigned int sz = Neighbors_arraysize < size ? Neighbors_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        Neighbors_var2[i] = this->Neighbors_var[i];
    Neighbors_arraysize = size;
    delete [] this->Neighbors_var;
    this->Neighbors_var = Neighbors_var2;
}

unsigned int StringPacket::getNeighborsArraySize() const
{
    return Neighbors_arraysize;
}

stringNeighbor& StringPacket::getNeighbors(unsigned int k)
{
    if (k>=Neighbors_arraysize) throw cRuntimeError("Array of size %d indexed by %d", Neighbors_arraysize, k);
    return Neighbors_var[k];
}

void StringPacket::setNeighbors(unsigned int k, const stringNeighbor& Neighbors)
{
    if (k>=Neighbors_arraysize) throw cRuntimeError("Array of size %d indexed by %d", Neighbors_arraysize, k);
    this->Neighbors_var[k] = Neighbors;
}

class StringPacketDescriptor : public cClassDescriptor
{
  public:
    StringPacketDescriptor();
    virtual ~StringPacketDescriptor();

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

Register_ClassDescriptor(StringPacketDescriptor);

StringPacketDescriptor::StringPacketDescriptor() : cClassDescriptor("StringPacket", "RoutingPacket")
{
}

StringPacketDescriptor::~StringPacketDescriptor()
{
}

bool StringPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<StringPacket *>(obj)!=NULL;
}

const char *StringPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int StringPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 11+basedesc->getFieldCount(object) : 11;
}

unsigned int StringPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *StringPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "Isource",
        "sinkPosition",
        "currentRound",
        "hopNumber",
        "sinkRssi",
        "location",
        "residual",
        "moisture",
        "StringPacketKind",
        "schedule",
        "Neighbors",
    };
    return (field>=0 && field<11) ? fieldNames[field] : NULL;
}

int StringPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='I' && strcmp(fieldName, "Isource")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sinkPosition")==0) return base+1;
    if (fieldName[0]=='c' && strcmp(fieldName, "currentRound")==0) return base+2;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopNumber")==0) return base+3;
    if (fieldName[0]=='s' && strcmp(fieldName, "sinkRssi")==0) return base+4;
    if (fieldName[0]=='l' && strcmp(fieldName, "location")==0) return base+5;
    if (fieldName[0]=='r' && strcmp(fieldName, "residual")==0) return base+6;
    if (fieldName[0]=='m' && strcmp(fieldName, "moisture")==0) return base+7;
    if (fieldName[0]=='S' && strcmp(fieldName, "StringPacketKind")==0) return base+8;
    if (fieldName[0]=='s' && strcmp(fieldName, "schedule")==0) return base+9;
    if (fieldName[0]=='N' && strcmp(fieldName, "Neighbors")==0) return base+10;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *StringPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "string",
        "int",
        "int",
        "int",
        "double",
        "double",
        "double",
        "double",
        "int",
        "int",
        "stringNeighbor",
    };
    return (field>=0 && field<11) ? fieldTypeStrings[field] : NULL;
}

const char *StringPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 8:
            if (!strcmp(propertyname,"enum")) return "StringPacket_Type";
            return NULL;
        default: return NULL;
    }
}

int StringPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    StringPacket *pp = (StringPacket *)object; (void)pp;
    switch (field) {
        case 9: return pp->getScheduleArraySize();
        case 10: return pp->getNeighborsArraySize();
        default: return 0;
    }
}

std::string StringPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    StringPacket *pp = (StringPacket *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getIsource());
        case 1: return long2string(pp->getSinkPosition());
        case 2: return long2string(pp->getCurrentRound());
        case 3: return long2string(pp->getHopNumber());
        case 4: return double2string(pp->getSinkRssi());
        case 5: return double2string(pp->getLocation());
        case 6: return double2string(pp->getResidual());
        case 7: return double2string(pp->getMoisture());
        case 8: return long2string(pp->getStringPacketKind());
        case 9: return long2string(pp->getSchedule(i));
        case 10: {std::stringstream out; out << pp->getNeighbors(i); return out.str();}
        default: return "";
    }
}

bool StringPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    StringPacket *pp = (StringPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setIsource((value)); return true;
        case 1: pp->setSinkPosition(string2long(value)); return true;
        case 2: pp->setCurrentRound(string2long(value)); return true;
        case 3: pp->setHopNumber(string2long(value)); return true;
        case 4: pp->setSinkRssi(string2double(value)); return true;
        case 5: pp->setLocation(string2double(value)); return true;
        case 6: pp->setResidual(string2double(value)); return true;
        case 7: pp->setMoisture(string2double(value)); return true;
        case 8: pp->setStringPacketKind(string2long(value)); return true;
        case 9: pp->setSchedule(i,string2long(value)); return true;
        default: return false;
    }
}

const char *StringPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 10: return opp_typename(typeid(stringNeighbor));
        default: return NULL;
    };
}

void *StringPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    StringPacket *pp = (StringPacket *)object; (void)pp;
    switch (field) {
        case 10: return (void *)(&pp->getNeighbors(i)); break;
        default: return NULL;
    }
}


