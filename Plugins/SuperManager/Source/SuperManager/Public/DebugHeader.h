#pragma once

#include "Misc/MessageDialog.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"

namespace SM_Debug
{
	static void PrintDebug(const FString& InString, const FColor& InColor)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, InColor, InString);
		}
	}

	static void PrintLog(const FString& InString)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *InString);
	}

	static EAppReturnType::Type ShowMessageDialog(const FText& InContent, const FText& InTitle, EAppMsgType::Type InType)
	{
		return FMessageDialog::Open(InType, InContent, InTitle);
	}

	static void ShowNotifyInfo(const FText& InContent, const FText& InTitle)
	{
		FNotificationInfo Info(InTitle);
		Info.bFireAndForget = true;		// 自动消失
		Info.ExpireDuration = 2.0f;		// 显示时间
		Info.bUseLargeFont = true;		// 使用大字体
		Info.bUseThrobber = true;		// 使用加载动画
		Info.bUseSuccessFailIcons = false;	// 使用成功失败图标
		Info.FadeInDuration = 0.5f;		// 淡入时间
		Info.FadeOutDuration = 0.5f;	// 淡出时间

		Info.SubText = InContent;		// 设置内容

		FSlateNotificationManager::Get().AddNotification(Info);	// 添加通知
	}
}

