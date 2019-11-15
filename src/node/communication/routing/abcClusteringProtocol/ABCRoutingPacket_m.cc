//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/abcClusteringProtocol/ABCRoutingPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "ABCRoutingPacket_m.h"

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

EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("ABCRoutingPacket_Type");
    if (!e) enums.getInstance()->add(e = new cEnum("ABCRoutingPacket_Type"));
    e->insert(ABC_ROUTING_ADV_PACKET, "ABC_ROUTING_ADV_PACKET");
    e->insert(ABC_ROUTING_JOIN_PACKET, "ABC_ROUTING_JOIN_PACKET");
    e->insert(ABC_ROUTING_TDMA_PACKET, "ABC_ROUTING_TDMA_PACKET");
    e->insert(ABC_ROUTING_DATA_PACKET, "ABC_ROUTING_DATA_PACKET");
);

Register_Class(ABCRoutingPacket);

ABCRoutingPacket::ABCRoutingPacket(const char *name, int kind) : ::RoutingPacket(name,kind)
{
    this->type_packet_var = 0;
    this->hop_numb_var = 0;
    this->sum_energy_var = 0;
    this->residual_energy_var = 0;
    this->min_energy_var = 0;
    this->ABCRoutingPacketKind_var = 0;
    schedule_arraysize = 0;
    this->schedule_var = 0;
    path_arraysize = 0;
    this->path_var = 0;
}

ABCRoutingPacket::ABCRoutingPacket(const ABCRoutingPacket& other) : ::RoutingPacket(other)
{
    schedule_arraysize = 0;
    this->schedule_var = 0;
    path_arraysize = 0;
    this->path_var = 0;
    copy(other);
}

ABCRoutingPacket::~ABCRoutingPacket()
{
    delete [] schedule_var;
    delete [] path_var;
}

ABCRoutingPacket& ABCRoutingPacket::operator=(const ABCRoutingPacket& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void ABCRoutingPacket::copy(const ABCRoutingPacket& other)
{
    this->type_packet_var = other.type_packet_var;
    this->hop_numb_var = other.hop_numb_var;
    this->sum_energy_var = other.sum_energy_var;
    this->residual_energy_var = other.residual_energy_var;
    this->min_energy_var = other.min_energy_var;
    this->ABCRoutingPacketKind_var = other.ABCRoutingPacketKind_var;
    delete [] this->schedule_var;
    this->schedule_var = (other.schedule_arraysize==0) ? NULL : new int[other.schedule_arraysize];
    schedule_arraysize = other.schedule_arraysize;
    for (unsigned int i=0; i<schedule_arraysize; i++)
        this->schedule_var[i] = other.schedule_var[i];
    delete [] this->path_var;
    this->path_var = (other.path_arraysize==0) ? NULL : new int[other.path_arraysize];
    path_arraysize = other.path_arraysize;
    for (unsigned int i=0; i<path_arraysize; i++)
        this->path_var[i] = other.path_var[i];
}

void ABCRoutingPacket::parsimPack(cCommBuffer *b)
{
    ::RoutingPacket::parsimPack(b);
    doPacking(b,this->type_packet_var);
    doPacking(b,this->hop_numb_var);
    doPacking(b,this->sum_energy_var);
    doPacking(b,this->residual_energy_var);
    doPacking(b,this->min_energy_var);
    doPacking(b,this->ABCRoutingPacketKind_var);
    b->pack(schedule_arraysize);
    doPacking(b,this->schedule_var,schedule_arraysize);
    b->pack(path_arraysize);
    doPacking(b,this->path_var,path_arraysize);
}

void ABCRoutingPacket::parsimUnpack(cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->type_packet_var);
    doUnpacking(b,this->hop_numb_var);
    doUnpacking(b,this->sum_energy_var);
    doUnpacking(b,this->residual_energy_var);
    doUnpacking(b,this->min_energy_var);
    doUnpacking(b,this->ABCRoutingPacketKind_var);
    delete [] this->schedule_var;
    b->unpack(schedule_arraysize);
    if (schedule_arraysize==0) {
        this->schedule_var = 0;
    } else {
        this->schedule_var = new int[schedule_arraysize];
        doUnpacking(b,this->schedule_var,schedule_arraysize);
    }
    delete [] this->path_var;
    b->unpack(path_arraysize);
    if (path_arraysize==0) {
        this->path_var = 0;
    } else {
        this->path_var = new int[path_arraysize];
        doUnpacking(b,this->path_var,path_arraysize);
    }
}

int ABCRoutingPacket::getType_packet() const
{
    return type_packet_var;
}

void ABCRoutingPacket::setType_packet(int type_packet)
{
    this->type_packet_var = type_packet;
}

int ABCRoutingPacket::getHop_numb() const
{
    return hop_numb_var;
}

void ABCRoutingPacket::setHop_numb(int hop_numb)
{
    this->hop_numb_var = hop_numb;
}

int ABCRoutingPacket::getSum_energy() const
{
    return sum_energy_var;
}

void ABCRoutingPacket::setSum_energy(int sum_energy)
{
    this->sum_energy_var = sum_energy;
}

int ABCRoutingPacket::getResidual_energy() const
{
    return residual_energy_var;
}

void ABCRoutingPacket::setResidual_energy(int residual_energy)
{
    this->residual_energy_var = residual_energy;
}

int ABCRoutingPacket::getMin_energy() const
{
    return min_energy_var;
}

void ABCRoutingPacket::setMin_energy(int min_energy)
{
    this->min_energy_var = min_energy;
}

int ABCRoutingPacket::getABCRoutingPacketKind() const
{
    return ABCRoutingPacketKind_var;
}

void ABCRoutingPacket::setABCRoutingPacketKind(int ABCRoutingPacketKind)
{
    this->ABCRoutingPacketKind_var = ABCRoutingPacketKind;
}

void ABCRoutingPacket::setScheduleArraySize(unsigned int size)
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

unsigned int ABCRoutingPacket::getScheduleArraySize() const
{
    return schedule_arraysize;
}

int ABCRoutingPacket::getSchedule(unsigned int k) const
{
    if (k>=schedule_arraysize) throw cRuntimeError("Array of size %d indexed by %d", schedule_arraysize, k);
    return schedule_var[k];
}

void ABCRoutingPacket::setSchedule(unsigned int k, int schedule)
{
    if (k>=schedule_arraysize) throw cRuntimeError("Array of size %d indexed by %d", schedule_arraysize, k);
    this->schedule_var[k] = schedule;
}

void ABCRoutingPacket::setPathArraySize(unsigned int size)
{
    int *path_var2 = (size==0) ? NULL : new int[size];
    unsigned int sz = path_arraysize < size ? path_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        path_var2[i] = this->path_var[i];
    for (unsigned int i=sz; i<size; i++)
        path_var2[i] = 0;
    path_arraysize = size;
    delete [] this->path_var;
    this->path_var = path_var2;
}

unsigned int ABCRoutingPacket::getPathArraySize() const
{
    return path_arraysize;
}

int ABCRoutingPacket::getPath(unsigned int k) const
{
    if (k>=path_arraysize) throw cRuntimeError("Array of size %d indexed by %d", path_arraysize, k);
    return path_var[k];
}

void ABCRoutingPacket::setPath(unsigned int k, int path)
{
    if (k>=path_arraysize) throw cRuntimeError("Array of size %d indexed by %d", path_arraysize, k);
    this->path_var[k] = path;
}

class ABCRoutingPacketDescriptor : public cClassDescriptor
{
  public:
    ABCRoutingPacketDescriptor();
    virtual ~ABCRoutingPacketDescriptor();

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

Register_ClassDescriptor(ABCRoutingPacketDescriptor);

ABCRoutingPacketDescriptor::ABCRoutingPacketDescriptor() : cClassDescriptor("ABCRoutingPacket", "RoutingPacket")
{
}

ABCRoutingPacketDescriptor::~ABCRoutingPacketDescriptor()
{
}

bool ABCRoutingPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ABCRoutingPacket *>(obj)!=NULL;
}

const char *ABCRoutingPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ABCRoutingPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 8+basedesc->getFieldCount(object) : 8;
}

unsigned int ABCRoutingPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<8) ? fieldTypeFlags[field] : 0;
}

const char *ABCRoutingPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "type_packet",
        "hop_numb",
        "sum_energy",
        "residual_energy",
        "min_energy",
        "ABCRoutingPacketKind",
        "schedule",
        "path",
    };
    return (field>=0 && field<8) ? fieldNames[field] : NULL;
}

int ABCRoutingPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "type_packet")==0) return base+0;
    if (fieldName[0]=='h' && strcmp(fieldName, "hop_numb")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "sum_energy")==0) return base+2;
    if (fieldName[0]=='r' && strcmp(fieldName, "residual_energy")==0) return base+3;
    if (fieldName[0]=='m' && strcmp(fieldName, "min_energy")==0) return base+4;
    if (fieldName[0]=='A' && strcmp(fieldName, "ABCRoutingPacketKind")==0) return base+5;
    if (fieldName[0]=='s' && strcmp(fieldName, "schedule")==0) return base+6;
    if (fieldName[0]=='p' && strcmp(fieldName, "path")==0) return base+7;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ABCRoutingPacketDescriptor::getFieldTypeString(void *object, int field) const
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
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<8) ? fieldTypeStrings[field] : NULL;
}

const char *ABCRoutingPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 5:
            if (!strcmp(propertyname,"enum")) return "ABCRoutingPacket_Type";
            return NULL;
        default: return NULL;
    }
}

int ABCRoutingPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ABCRoutingPacket *pp = (ABCRoutingPacket *)object; (void)pp;
    switch (field) {
        case 6: return pp->getScheduleArraySize();
        case 7: return pp->getPathArraySize();
        default: return 0;
    }
}

std::string ABCRoutingPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ABCRoutingPacket *pp = (ABCRoutingPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getType_packet());
        case 1: return long2string(pp->getHop_numb());
        case 2: return long2string(pp->getSum_energy());
        case 3: return long2string(pp->getResidual_energy());
        case 4: return long2string(pp->getMin_energy());
        case 5: return long2string(pp->getABCRoutingPacketKind());
        case 6: return long2string(pp->getSchedule(i));
        case 7: return long2string(pp->getPath(i));
        default: return "";
    }
}

bool ABCRoutingPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ABCRoutingPacket *pp = (ABCRoutingPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setType_packet(string2long(value)); return true;
        case 1: pp->setHop_numb(string2long(value)); return true;
        case 2: pp->setSum_energy(string2long(value)); return true;
        case 3: pp->setResidual_energy(string2long(value)); return true;
        case 4: pp->setMin_energy(string2long(value)); return true;
        case 5: pp->setABCRoutingPacketKind(string2long(value)); return true;
        case 6: pp->setSchedule(i,string2long(value)); return true;
        case 7: pp->setPath(i,string2long(value)); return true;
        default: return false;
    }
}

const char *ABCRoutingPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *ABCRoutingPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ABCRoutingPacket *pp = (ABCRoutingPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


