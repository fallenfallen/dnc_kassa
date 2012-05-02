/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2011, Authors: Savitskaya Alexandra.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Савицкая Александра.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

#include "ObjectFilters.h"
#include "Feature.h"
#include "FeatureMapper.h"
#include "Ware.h"
#include "WareMapper.h"
#include "FeatureValueMapper.h"
#include "BarcodeMapper.h"
#include "AdditionMapper.h"
#include "WareGroup.h"
#include "WareGroupMapper.h"
#include "Section.h"
#include "SectionMapper.h"
#include "UserProfile.h"
#include "UserProfileMapper.h"
#include "UserMapper.h"
//-----(+)-----Zuskin-----15/12/2011-----
#include "DiscSchemeMapper.h"
//---------------------------------------

/********************************************************/

WareFilterByCode::WareFilterByCode(const string& wcode) {
    this->ware_code = wcode;
}
bool WareFilterByCode::compareObject(Object* obj) const {
    return ((Ware*) obj)->getCode() == this->ware_code;
}
string WareFilterByCode::getClassName() const {
    return "Ware";
}
string WareFilterByCode::getFilterExpression() const {
  RDBWhereFilter filter;
  filter.addWhereCondition(WARE_INTERNAL_CODE, EQUAL, ware_code);
  return filter.getWhereExpression();
}

/********************************************************/

FeaturesInSetFilter::FeaturesInSetFilter(int fset_id) {
    this->set_id = fset_id;
}
bool FeaturesInSetFilter::compareObject(Object* obj) const {
    return ((Feature*)obj)->isParentSet(set_id);
}
string FeaturesInSetFilter::getClassName() const {
    return "Features";
}
string FeaturesInSetFilter::getFilterExpression() const {
    RDBWhereFilter filter;
    filter.addWhereCondition(FEATURE_SET_ID, EQUAL, set_id);
    return filter.getWhereExpression();
}

/********************************************************/

AdditionsInWareFilter::AdditionsInWareFilter(int ware_oid) {
    this->ware_id = ware_oid;
}
string AdditionsInWareFilter::getClassName() const {
    return "Additions";
}
string AdditionsInWareFilter::getFilterExpression() const {
    RDBWhereFilter filter;
    filter.addWhereCondition(ADDITION_WARE_ID, EQUAL, ware_id);
    return filter.getWhereExpression();
}
bool AdditionsInWareFilter::compareObject(Object* obj) const {
    //return ((WareAddition*)obj)->getInfo().ware_id == ware_id;
    return true;
}


AdditionInWareByCodeFilter::AdditionInWareByCodeFilter(int ware_oid, const string& addit_code)
: AdditionsInWareFilter(ware_oid) {
    addition_code = addit_code;
}
string AdditionInWareByCodeFilter::getFilterExpression() const {
    RDBWhereFilter filter;
    filter.addWhereCondition(ADDITION_WARE_ID, EQUAL, ware_id);
    filter.addWhereCondition(ADDITION_CODE, EQUAL, addition_code);
    return filter.getWhereExpression();
}

/********************************************************/

SelectedValueFilter::SelectedValueFilter(int addition_id, int feature_id) {
    this->feature_id = feature_id;
    this->addition_id = addition_id;
}
bool SelectedValueFilter::compareObject(Object* obj) const {
    return ((SelectedValue*)obj)->getFeatureId() == feature_id;
}
string SelectedValueFilter::getClassName() const {
    return "SelectedValue";
}
string SelectedValueFilter::getFilterExpression() const {
    RDBWhereFilter filter;
    filter.addWhereCondition(SFVALUE_FEATURE_ID, EQUAL, feature_id);
    filter.addWhereCondition(SFVALUE_ADDITION_ID, EQUAL, addition_id);
    return filter.getWhereExpression();
}

/********************************************************/

string BarcodeFilter::getClassName() const {
    return "Barcode";
}

/********************************************************/

AdditionBarcodeFilter::AdditionBarcodeFilter(const string &addition_code) {
    this->addtion_code = addition_code;
}
bool AdditionBarcodeFilter::compareObject(Object* obj) const {
    return ((Barcode*)obj)->getAdditionCode() == addtion_code;
}
string AdditionBarcodeFilter::getFilterExpression() const {
    RDBWhereFilter filter;
    filter.addWhereCondition(BARCODE_ADDITION_CODE, EQUAL, addtion_code);
    return filter.getWhereExpression();
}

/********************************************************/

WareBarcodeFilter::WareBarcodeFilter(const string& ware_code) {
    this->ware_code = ware_code;
}
bool WareBarcodeFilter::compareObject(Object* obj) const {
    return ((Barcode*)obj)->getWareInternalCode() == ware_code;
}
string WareBarcodeFilter::getFilterExpression() const {
    RDBWhereFilter filter;
    filter.addWhereCondition(BARCODE_WARE_CODE, EQUAL, ware_code);
    return filter.getWhereExpression();
}

/********************************************************/

ExistAdditionBarcodeFilter::ExistAdditionBarcodeFilter(const string& addition_code, const string& barcode_text){
    this->addtion_code = addition_code;
    this->barcode = barcode_text;
}
bool ExistAdditionBarcodeFilter::compareObject(Object* obj) const {
    if ( ((Barcode*)obj)->getAdditionCode() != addtion_code )
        return false;
    if ( ((Barcode*)obj)->getText() != barcode )
        return false;
    return true;
}
string ExistAdditionBarcodeFilter::getFilterExpression() const {
    RDBWhereFilter filter;
    filter.addWhereCondition(BARCODE_ADDITION_CODE, EQUAL, addtion_code);
    filter.addWhereCondition(BARCODE_TEXT, EQUAL, barcode);
    return filter.getWhereExpression();
}

/********************************************************/

ExistWareBarcodeFilter::ExistWareBarcodeFilter(const string& ware_internal_code, const string& barcode_text) {
    ware_code = ware_internal_code;
    barcode = barcode_text;
}
bool ExistWareBarcodeFilter::compareObject(Object* obj) const {
    if ( ((Barcode*)obj)->getWareInternalCode() != ware_code )
        return false;
    if ( ((Barcode*)obj)->getText() != barcode )
        return false;
    return true;
}
string ExistWareBarcodeFilter::getFilterExpression() const {
    RDBWhereFilter filter;
    filter.addWhereCondition(BARCODE_WARE_CODE, EQUAL, ware_code);
    filter.addWhereCondition(BARCODE_TEXT, EQUAL, barcode);
    return filter.getWhereExpression();
}

/********************************************************/
FeatureTieWithSetFilter::FeatureTieWithSetFilter(int f_id, int s_id) {
    this->f_id = f_id;
    this->s_id = s_id;
}
bool FeatureTieWithSetFilter::compareObject(Object* obj) const {
    FeatureTieWithSet* fis_object = (FeatureTieWithSet*)obj;
    if (fis_object->feature_id != f_id)
        return false;
    if ( fis_object->set_id != s_id )
        return false;
    return true;
}
string FeatureTieWithSetFilter::getClassName() const {
    return "FeaturesInSet";
}
string FeatureTieWithSetFilter::getFilterExpression() const {
    RDBWhereFilter filter;
    filter.addWhereCondition(FIS_FEATURE_ID, EQUAL, f_id);
    filter.addWhereCondition(FIS_SET_ID, EQUAL, s_id);
    return filter.getWhereExpression();
}

bool AllFISFilter::compareObject(Object* obj) const {
    return true;
}
string AllFISFilter::getClassName() const {
    return "FeaturesInSet";
}
string AllFISFilter::getFilterExpression() const {
    return "";
}

/********************************************************/
SelectedValueByAdditionFilter::SelectedValueByAdditionFilter(int addition_id, int value_id) {
    saddition_id = addition_id;
    svalue_id = value_id;
}
bool SelectedValueByAdditionFilter::compareObject(Object* obj) const {
    if ( ((SelectedValue*)obj)->getAdditionId() != saddition_id )
        return false;
    if ( ((SelectedValue*)obj)->getValueId() != svalue_id )
        return false;
    return true;
}
string SelectedValueByAdditionFilter::getClassName() const {
    return "SelectedValue";
}
string SelectedValueByAdditionFilter::getFilterExpression() const {
    RDBWhereFilter filter;
    filter.addWhereCondition(SFVALUE_VALUE_ID, EQUAL, svalue_id);
    filter.addWhereCondition(SFVALUE_ADDITION_ID, EQUAL, saddition_id);
    return filter.getWhereExpression();
}

/********************************************************/

bool AllWareWithFS::compareObject(Object* obj) const {
    return ((Ware*)obj)->isUseFeaturesSet();
}
string AllWareWithFS::getClassName() const {
    return "Ware";
}
string AllWareWithFS::getFilterExpression() const {
    RDBWhereFilter filter;
    filter.addWhereCondition(WARE_FSET_ID, GREATER, 0);
    //filter.addWhereCondition(WARE_ID_FIELD, EQUAL, 32720);
    return filter.getWhereExpression();
}

/********************************************************/


AllValueInAdditionFilter::AllValueInAdditionFilter(int addition_id) {
    f_addition_id = addition_id;
}
bool AllValueInAdditionFilter::compareObject(Object* obj) const {
    return ((SelectedValue*)obj)->getAdditionId() == f_addition_id;
}
string AllValueInAdditionFilter::getClassName() const {
    return "SelectedValue";
}
string AllValueInAdditionFilter::getFilterExpression() const {
    RDBWhereFilter filter;
    filter.addWhereCondition(SFVALUE_ADDITION_ID, EQUAL, f_addition_id);
    return filter.getWhereExpression();
}

/********************************************************/

bool AllObjectInClass::compareObject(Object* obj) const {
    return true;
}
string AllObjectInClass::getFilterExpression() const {
    return "";
}

string AllFeaturesFilter::getClassName() const {
    return "Features";
}

string AllFeaturesSetsFilter::getClassName() const {
    return "FeaturesSet";
}

string AllWaresFilter::getClassName() const {
    return "Ware";
}

string AllWareGroupsFilter::getClassName() const {
    return "WareGroup";
}

/********************************************************/

PossibleValueInFeature::PossibleValueInFeature(int feature_id) {
    this->feature_id = feature_id;
}
bool PossibleValueInFeature::compareObject(Object* obj) const {
    return ((FeatureValue*)obj)->getFeatureId() == this->feature_id;
}
string PossibleValueInFeature::getClassName() const {
    return "FeatureValue";
}
string PossibleValueInFeature::getFilterExpression() const {
    RDBWhereFilter filter;
    filter.addWhereCondition(FVALUE_FEATURE_ID, EQUAL, feature_id);
    return filter.getWhereExpression();
}

/********************************************************/

GroupSubItemFilter::GroupSubItemFilter(int parent_group_id) {
    this->parent_id = parent_group_id;
}
string GroupSubItemFilter::getClassName() const {
    return "WareGroup";
}
bool GroupSubItemFilter::compareObject(Object* obj) const {
    return ((WareGroup*)obj)->getParentId() == this->parent_id;
}
string GroupSubItemFilter::getFilterExpression() const {
    RDBWhereFilter filter;
    filter.addWhereCondition(GROUP_PARENT_ID, EQUAL, parent_id);
    return filter.getWhereExpression();
}


GroupSubWareFilter::GroupSubWareFilter(int parent_group_id)
        : GroupSubItemFilter(parent_group_id) { }
string GroupSubWareFilter::getClassName() const {
    return "Ware";
}
bool GroupSubWareFilter::compareObject(Object* obj) const {
    return ((Ware*)obj)->getGroupId() == this->parent_id;
}
string GroupSubWareFilter::getFilterExpression() const {
    RDBWhereFilter filter;
    filter.addWhereCondition(WARE_GROUP_ID, EQUAL, parent_id);
    return filter.getWhereExpression();
}

/********************************************************/

bool FiscalSectionFilter::compareObject(Object* obj) const {
    return ((Section*)obj)->isFiscal();
}
string FiscalSectionFilter::getClassName() const {
    return "Section";
}
string FiscalSectionFilter::getFilterExpression() const {
    RDBWhereFilter filter;
    filter.addWhereCondition(SECTION_IS_FISCAL, EQUAL, "t");
    return filter.getWhereExpression();
}



bool NotFiscalSectionFilter::compareObject(Object* obj) const {
    return ! ((Section*)obj)->isFiscal();
}
string NotFiscalSectionFilter::getClassName() const {
    return "Section";
}
string NotFiscalSectionFilter::getFilterExpression() const {
    RDBWhereFilter filter;
    filter.addWhereCondition(SECTION_IS_FISCAL, NOT_EQUAL, "t");
    return filter.getWhereExpression();
}


SectionByNumberFilter::SectionByNumberFilter(int section_num) {
    this->section_number = section_num;
}
bool SectionByNumberFilter::compareObject(Object* obj) const {
    return ((Section*)obj)->getNumber() == this->section_number;
}
string SectionByNumberFilter::getClassName() const {
    return "Section";
}
string SectionByNumberFilter::getFilterExpression() const {
    RDBWhereFilter filter;
    filter.addWhereCondition(SECTION_NUMBER, EQUAL, this->section_number);
    return filter.getWhereExpression();
}

/********************************************************/

bool AllProfilesWithoutAdmin::compareObject(Object* obj) const {
    return ((UserProfile*)obj)->getOID() != 1;
}
string AllProfilesWithoutAdmin::getClassName() const {
    return "Profile";
}
string AllProfilesWithoutAdmin::getFilterExpression() const {
    RDBWhereFilter filter;
    filter.addWhereCondition(PROFILE_ID, NOT_EQUAL, 1);
    return filter.getWhereExpression();
}

bool AllUsersWithoutAdmin::compareObject(Object* obj) const {
    UserInfo user_info = ((User*)obj)->getInfo();
    return user_info.profile_id != 1;
}
string AllUsersWithoutAdmin::getClassName() const {
    return "User";
}
string AllUsersWithoutAdmin::getFilterExpression() const {
    RDBWhereFilter filter;
    filter.addWhereCondition(USER_PROFILE_ID, NOT_EQUAL, 1);
    return filter.getWhereExpression();
}

/********************************************************/
//-----(+)-----Zuskin-----15/12/2011-----
bool DefaultDiscSchemeFilter::compareObject(Object* obj) const {
    return ((DiscScheme*)obj)->getIsDefault();
}
string DefaultDiscSchemeFilter::getClassName() const {
    return "DiscScheme";
}
string DefaultDiscSchemeFilter::getFilterExpression() const {
    RDBWhereFilter filter;
    filter.addWhereCondition(DISCSCHEME_IS_DEFAULT, EQUAL, "t");
    return filter.getWhereExpression();
}

DiscSchemeByNumberFilter::DiscSchemeByNumberFilter(int discscheme_number) {
    this->discscheme_number = discscheme_number;
}
bool DiscSchemeByNumberFilter::compareObject(Object* obj) const {
    return ((DiscScheme*)obj)->getOID() == this->discscheme_number;
}
string DiscSchemeByNumberFilter::getClassName() const {
    return "DiscScheme";
}
string DiscSchemeByNumberFilter::getFilterExpression() const {
    RDBWhereFilter filter;
    filter.addWhereCondition(DISCSCHEME_CODE, EQUAL, this->discscheme_number);
    return filter.getWhereExpression();
}

//----------------------------------------
