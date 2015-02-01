<?php
namespace aquagear\utility;

/** 
 * HTTP用ユーティリティクラス.
 * 
 * @package aquagear
 * @subpackage utility
 * 
 * @author motonari
 * @version $Id: HttpUtility.inc 535 2011-07-26 14:02:27Z motonari $
 */
class HttpUtility extends \aquagear\Object {	
	/**
	 * ヘッダーの送信
	 * 
	 * @param string $header ヘッダの文字列
	 * 
	 * @return void
	 */
	public static function sendHeader($header) {
		header($header);
	}
	
	/**
	 * redirect処理.
	 * 
	 * @param string $url_str リダイレクト先
	 * @return void
	 */
	public static function redirect($url_str){
		\aquagear\log\Logger::getLogInstance()->info('redirect url [url = ' . $url_str . ']');
		header("Location: $url_str");
		exit;
	}
	
	/**
	 * エラーページへのリダイレクト
	 * 
	 * @return void
	 */
	public static function errorRedirect() {
		\aquagear\log\Logger::finalize();
		$error_configure = \aquagear\application\ApplicationData::getSystemConfigure('error');
		if(\aquagear\application\ApplicationData::getAquagearServerEnv('debug') !== 'on') {
			\aquagear\utility\HttpUtility::redirect($error_configure['error_url']);
		}
	}
	
	/**
	 * ステータスコードからヘッダ情報の取得
	 * 
	 * @param int $status_code ステータスコード
	 * 
	 * @return string ヘッダー文字列
	 */
	public static function getHttpStatusHeader($status_code) {
		switch($status_code) {
			case 200:
			case 404:
			case 301:
			default:
				return '';
		}
	}
}
?>