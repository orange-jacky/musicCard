/**
 * 
 */
package com.resoft;

/**
 * @author user
 *
 */
public class ResoftVoice {

	private native int resoftJniCall(InputParam inputparam, OutputResult outputresult);
	
	static {
		System.loadLibrary("resoft_voice_card");
	}
	
	public  int resoftVoiceCall(InputParam inputparam, OutputResult outputresult){
			return resoftJniCall(inputparam, outputresult);
	}
}
