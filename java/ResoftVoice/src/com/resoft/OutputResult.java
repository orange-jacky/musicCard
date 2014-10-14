package com.resoft;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class OutputResult {
	
	public String alertCode;
	public ArrayList<String> alertDialResult;//姓名|电话号码|返回值
	//返回值对应关系如下：
	//-1----打通后，对方在摘机超时时间内没接
	//-2----打通电话后，在播放字符串超时时间内没有播放完字符串
	//正数表示打通电话后，语音播放字符串的时间，单位是秒
	
	public String attribute1;
	public String attribute2;
	
	public String getAlertCode() {
		return alertCode;
	}
	public void setAlertCode(String alertCode) {
		this.alertCode = alertCode;
	}
	public ArrayList<String> getAlertDialResult() {
		return alertDialResult;
	}
	public void setAlertDialResult(ArrayList<String> alertDialResult) {
		this.alertDialResult = alertDialResult;
	}
	public String getAttribute1() {
		return attribute1;
	}
	public void setAttribute1(String attribute1) {
		this.attribute1 = attribute1;
	}
	public String getAttribute2() {
		return attribute2;
	}
	public void setAttribute2(String attribute2) {
		this.attribute2 = attribute2;
	}
}
