// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "World/CQWorldManager.h"

ACQWorldManager::ACQWorldManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

FText ACQWorldManager::GetAreaDisplayName(ECQWorldArea Area)
{
	switch (Area)
	{
	case ECQWorldArea::CrystalTown:
		return FText::FromString(TEXT("晶石小镇"));
	case ECQWorldArea::PhantomForest:
		return FText::FromString(TEXT("幽光森林"));
	case ECQWorldArea::MoltenCanyon:
		return FText::FromString(TEXT("熔烬峡谷"));
	default:
		return FText::FromString(TEXT("未知区域"));
	}
}

FText ACQWorldManager::GetAreaDescription(ECQWorldArea Area)
{
	switch (Area)
	{
	case ECQWorldArea::CrystalTown:
		return FText::FromString(TEXT("一个被晶石能量保护的宁静小镇，是训练师们的起点。"));
	case ECQWorldArea::PhantomForest:
		return FText::FromString(TEXT("弥漫着神秘幽光的古老森林，栖息着许多草属性和暗属性的晶兽。"));
	case ECQWorldArea::MoltenCanyon:
		return FText::FromString(TEXT("炽热的峡谷深处涌动着岩浆，强大的火属性晶兽在此守护着净化之力。"));
	default:
		return FText::GetEmpty();
	}
}
