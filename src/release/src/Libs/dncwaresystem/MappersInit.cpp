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

#include "MappersInit.h"
#include "MapperContainer.h"

#include "WareMapper.h"
#include "AdditionMapper.h"
#include "FeaturesSetMapper.h"
#include "FeatureMapper.h"
#include "FeatureValueMapper.h"
#include "BarcodeMapper.h"
#include "WareGroupMapper.h"
#include "SectionMapper.h"
#include "FixedDiscountMapper.h"
#include "UserProfileMapper.h"
#include "UserMapper.h"
//-----(+)-----Zuskin-----18/07/2011-----
#include "DiscSchemeMapper.h"
#include "DiscRuleMapper.h"
//---------------------------------------
//-----(+)-----Zuskin-----21/11/2011-----
#include "ManagerMapper.h"
//---------------------------------------
//-----(+)-----Zuskin-----22/11/2011-----
#include "TaxGroupMapper.h"
//---------------------------------------
//-----(+)-----Zuskin-----11/02/2012-----
#include "TaxRateMapper.h"
#include "TaxRateGrMapper.h"
//---------------------------------------


void MappersInit::initMappers() {
    MapperContainer::addMapper("Ware", new WareMapper);
    MapperContainer::addMapper("Additions", new AdditionMapper);
    MapperContainer::addMapper("FeaturesSet", new FeaturesSetMapper);
    MapperContainer::addMapper("Features", new FeatureMapper);
    MapperContainer::addMapper("FeatureValue", new FeatureValueMapper);
    MapperContainer::addMapper("SelectedValue", new SelectedValueMapper);
    MapperContainer::addMapper("Barcode", new BarcodeMapper);
    MapperContainer::addMapper("FeaturesInSet", new FeatureTieWithSetMapper);
    MapperContainer::addMapper("WareGroup", new WareGroupMapper);
    MapperContainer::addMapper("Section", new SectionMapper);
    MapperContainer::addMapper("FixedDiscount", new FixedDiscountMapper);
    MapperContainer::addMapper("Profile", new UserProfileMapper);
    MapperContainer::addMapper("User", new UserMapper);
    //-----(+)-----Zuskin-----18/07/2011-----
    MapperContainer::addMapper("DiscScheme", new DiscSchemeMapper);
    MapperContainer::addMapper("DiscRule", new DiscRuleMapper);
    //---------------------------------------
    //-----(+)-----Zuskin-----21/11/2011-----
    MapperContainer::addMapper("Manager", new ManagerMapper);
    //---------------------------------------
    //-----(+)-----Zuskin-----22/11/2011-----
    MapperContainer::addMapper("TaxGroup", new TaxGroupMapper);
    //---------------------------------------
    //-----(+)-----Zuskin-----11/02/2012-----
    MapperContainer::addMapper("TaxRates", new TaxRateMapper);
    MapperContainer::addMapper("TaxRateGr", new TaxRateGrMapper);
    //---------------------------------------
}

