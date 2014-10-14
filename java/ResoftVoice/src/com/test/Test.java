package com.test;


import java.io.UnsupportedEncodingException;
import java.util.ArrayList;

import com.resoft.InputParam;
import com.resoft.OutputResult;
import com.resoft.ResoftVoice;


public class Test {

	
	/**
	 * @param args
	 * @throws UnsupportedEncodingException 
	 * @throws InterruptedException 
	 */
	public static void main(String[] args) throws UnsupportedEncodingException, InterruptedException {
		InputParam inputParam = new InputParam();		
		OutputResult outputResult = new OutputResult();

		inputParam.alertCode = "10000";
		//inputParam.alertSummary = "给[gei3]你[ni1]一张过去的[de4]CD,听听那[na4]时[shi2]我[wo3]们的爱[ai4]情[qing2]，有[you3]时[shi2]会突然[ran2]忘[wang4]了 我还在爱着你";
		inputParam.alertSummary = "系统帐户和管理员帐户 （管理员组） 有相同的文件权限，但它们具有不同的功能。通过操作系统以及在 Windows 下运行的服务使用系统帐户。有许多服务和需要的能力 （例如 Windows 安装期间） 内部登录的 Windows 中的进程。系统帐户专为此目的 ；它是个内部帐户、 不显示在用户管理器中，不能添加到任何组中，并且不能具有用户权限分配给它。另一方面，系统帐户上 NTFS 卷在文件管理器中的安全菜单上的权限部分中显示。默认情况下，系统帐户授予对 NTFS 卷上的所有文件的完全控制。这里的系统帐户有与管理员帐户同样的操作特权 ";

		String userinfo1 = "李晓明|9,18614031161";
		//String userinfo2 = "王鑫|915010035001";
		//String userinfo3 = "牛华伟|9,15600243182";
		//String userinfo4 = "徐立元|9,18618150708";
		//String userinfo5 = "贾晓玉|9,15110159103";		
		
		inputParam.alertUserInfo = new ArrayList<String>();
		inputParam.alertUserInfo.add(userinfo1);
		//inputParam.alertUserInfo.add(userinfo2);
		//inputParam.alertUserInfo.add(userinfo3);		
		//inputParam.alertUserInfo.add(userinfo4);
		//inputParam.alertUserInfo.add(userinfo5);
		
		inputParam.attribute1 = "reserver1";
		inputParam.attribute2 = "reserver2";
		
		System.out.println("In Java, inputParam.alertCode is " + inputParam.alertCode);
		System.out.println("In Java, inputParam.alertSummary is " + inputParam.alertSummary);
		System.out.println("In Java, inputParam.alertUserInfo is " + inputParam.alertUserInfo.toString());
		System.out.println("In Java, inputParam.attribute1 is " + inputParam.attribute1);
		System.out.println("In Java, inputParam.attribute2 is " + inputParam.attribute2);		

		
		ResoftVoice resoftvoice =  new ResoftVoice();
		System.out.println("In Java, return code is " + resoftvoice.resoftVoiceCall(inputParam, outputResult));		
		System.out.println("In Java, outputResult.alertCode is " + outputResult.alertCode);
		System.out.println("In Java, outputResult.attribute1 is " + outputResult.attribute1);
		System.out.println("In Java, outputResult.attribute2 is " + outputResult.attribute2);
		for(;;){
			if(outputResult.alertDialResult.size() == inputParam.alertUserInfo.size()){
				System.out.println("In Java, outputResult.alertDialResult is " + outputResult.alertDialResult.toString());
				break;
			}
		}
	}

}
