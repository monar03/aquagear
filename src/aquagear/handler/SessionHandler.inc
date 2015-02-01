<?php 
namespace aquagear\handler;

/**
 * セッションのハンドリングクラス.<br /> 
 * セッション関係はこのクラスのメソッドにて処理する.
 * 
 * @package aquagear 
 * @subpackage handler
 * 
 * @author motonari
 * @version $Id: SessionHandler.inc 535 2011-07-26 14:02:27Z motonari $
 */
class SessionHandler extends CoreHandler {
	/**
	 * 設定ファイルオブジェクト
	 * 
	 * @var object
	 */
	static private $_conf_obj = null;
	
	/**
	 * セッション名
	 * 
	 * @var string
	 */
	static private $_session_name = null;
	
	/**
	 * ハンドラの初期化.
	 * 
	 * @return void
	 */
	public static function initHandler() {
		self::$_conf_obj = \aquagear\holder\ConfigureHolder::load(\aquagear\application\ApplicationData::getAquagearServerEnv('system_configure_root') . "/session.ini");
		if(self::$_conf_obj->getValue('session')) {			
			@session_set_save_handler (array('\aquagear\handler\SessionHandler', 'open'),
										array('\aquagear\handler\SessionHandler', 'close'),
										array('\aquagear\handler\SessionHandler', 'read'),
										array('\aquagear\handler\SessionHandler', 'write'),
										array('\aquagear\handler\SessionHandler', 'destroy'),
										array('\aquagear\handler\SessionHandler', 'gc'));
			session_start();
		}
	}
	
	/**
	 * セッションを開く.
	 * 
	 * @param string $save_path    セッションのセーブパス
	 * @param string $session_name セッション名
	 * @return bool
	 * 
	 * @see http://jp2.php.net/manual/ja/function.session-set-save-handler.php
	 */
	public static function open($save_path, $session_name) {		
		self::$_session_name = $session_name;
		
		return true;
	}
	
	/**
	 * セッションを閉じるときに呼ばれる.
	 * 
	 * @return bool
	 * 
	 * @see http://jp2.php.net/manual/ja/function.session-set-save-handler.php
	 */
	public static function close() {
		return true;
	}
	
	/**
	 * セッションの呼び出し.
	 * 
	 * @param string $id セッションID
	 * @return string
	 * 
	 * @see http://jp2.php.net/manual/ja/function.session-set-save-handler.php
	 */
	public static function read($id) {
		$data = "";
		if(is_file(self::$_conf_obj->getValue('session_dir') . "/" . self::$_conf_obj->getValue('prefix') . "_" . md5(self::$_session_name . $id))) {
			\aquagear\log\Logger::getLogInstance()->debug('get session data [file_path = '.self::$_conf_obj->getValue('session_dir') . "/" . self::$_conf_obj->getValue('prefix') . "_" . md5(self::$_session_name . $id).']');
			$data = \aquagear\file\FileReaderWriter::readByString(self::$_conf_obj->getValue('session_dir') . "/" . self::$_conf_obj->getValue('prefix') . "_" . md5(self::$_session_name . $id));
		}
		
		return $data;
	}
	
	/**
	 * セッションの書き込み.
	 * 
	 * @param string $id           セッションID
	 * @param string $session_data セッションデータ
	 * @return bool
	 * 
	 * @see http://jp2.php.net/manual/ja/function.session-set-save-handler.php
	 */
	public static function write($id, $session_data) {
		\aquagear\log\Logger::getLogInstance()->debug('write session data [file_path = '.self::$_conf_obj->getValue('session_dir') . "/" . self::$_conf_obj->getValue('prefix') . "_" . md5(self::$_session_name . $id).']');
		\aquagear\file\FileReaderWriter::write(self::$_conf_obj->getValue('session_dir') . "/" . self::$_conf_obj->getValue('prefix') . "_" . md5(self::$_session_name . $id), $session_data, LOCK_EX, true);

		return true;
	}
	
	/**
	 * セッションの削除時に呼ばれる.
	 * 
	 * @param string $id セッションID
	 * @return bool
	 * 
	 * @see http://jp2.php.net/manual/ja/function.session-set-save-handler.php
	 */
	public static function destroy($id) {
		
		if(is_file(self::$_conf_obj->getValue('session_dir') . "/" . self::$_conf_obj->getValue('prefix') . "_" . md5(self::$_session_name . $id))) {
			\aquagear\log\Logger::getLogInstance()->debug('destroy session data [file_path = '.self::$_conf_obj->getValue('session_dir') . "/" . self::$_conf_obj->getValue('prefix') . "_" . md5(self::$_session_name . $id).']');
			\aquagear\file\FileOperator::rm(self::$_conf_obj->getValue('session_dir') . "/" . self::$_conf_obj->getValue('prefix') . "_" . md5(self::$_session_name . $id), true);
		}
		
		return true;
	}
	
	/**
	 * セッションをガーベジする.
	 * 
	 * @param int $lifetime 存続時間
	 * @return bool
	 */
	public static function gc($lifetime) {
		return true;
	}
}
?>