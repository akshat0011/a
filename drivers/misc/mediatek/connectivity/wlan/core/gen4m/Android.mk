LOCAL_PATH := $(call my-dir)

ifeq ($(MTK_WLAN_SUPPORT), yes)

include $(CLEAR_VARS)
LOCAL_MODULE := wlan_drv_gen4m.ko
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
LOCAL_REQUIRED_MODULES := wmt_chrdev_wifi.ko

include $(MTK_KERNEL_MODULE)

WIFI_NAME := wlan_drv_gen4m
WIFI_OPTS := CONFIG_MTK_COMBO_WIFI_HIF=$(WIFI_HIF) MODULE_NAME=$(WIFI_NAME) MTK_COMBO_CHIP=$(WIFI_CHIP) WLAN_CHIP_ID=$(WLAN_CHIP_ID) MTK_ANDROID_WMT=$(WIFI_WMT) WIFI_ENABLE_GCOV=$(WIFI_ENABLE_GCOV) WIFI_IP_SET=$(WIFI_IP_SET) MTK_ANDROID_EMI=$(WIFI_EMI)
WIFI_OPTS += MTK_SISO_SUPPORT=$(MTK_SISO_SUPPORT)
WIFI_OPTS += MTK_WLAN_SERVICE=yes

$(linked_module): OPTS += $(WIFI_OPTS)

endif
