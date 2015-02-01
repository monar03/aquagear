<?php
namespace aquagear\application;

/**
 * アプリケーションの統括クラス.
 * 
 * @package aquagear
 * @subpackage application
 * 
 * @author motonari
 * @version $Id: Application.inc 538 2011-07-31 12:52:00Z motonari $
 *
 */ 
use aquagear\holder\HttpParameterHolder;

class Application extends \aquagear\Object {
	/**
	 * アプリケーションスタック.
	 * 
	 * @var array
	 */
	private static $_application_stack = null;
	
	/**
	 * stackキャッシュ用serialize文字列
	 * 
	 * @var string
	 */
	private static $_cache_str = null;
	
	/**
	 * 初期化.
	 * 
	 * @return void
	 */
	private static function _initApplication() {
		self::_setHandler();
		self::_initStack();		
	}
	
	/**
	 * finalize
	 * 
	 * @return void
	 */
	private static function _finalizeApplication() {
		if(\aquagear\application\ApplicationData::getAquagearServerEnv('cache') !== 'on' &&
			!empty(self::$_cache_str) &&
			\aquagear\cache\Cache::getCacheInstance('stack')->cacheExists(\aquagear\holder\HttpParameterHolder::getInstance()->getURI()) === false) {
			\aquagear\cache\Cache::getCacheInstance('stack')->setValue(\aquagear\holder\HttpParameterHolder::getInstance()->getURI(), self::$_cache_str);
		}
	}
	
	/**
	 * アプリケーションスタックの初期化.
	 * 
	 * @return void
	 */
	private static function _initStack() {
		\aquagear\log\Logger::getLogInstance()->debug('start init application stack.');
		
		$stack_file_path = \aquagear\application\ApplicationData::getApplicationConfDir() . "/stack.ini";
		if(! is_file($stack_file_path)) {
			$stack_file_path = \aquagear\application\ApplicationData::getAquagearServerEnv('system_configure_root') . "/stack.ini";
		}
		
		$application_stack = unserialize(\aquagear\cache\Cache::getCacheInstance('stack')->getValue(md5(\aquagear\holder\HttpParameterHolder::getInstance()->getURI())));
		if(\aquagear\application\ApplicationData::getAquagearServerEnv('cache') !== 'on' && 
			!empty($application_stack)) {
			self::$_application_stack = $application_stack;
			return ;
		}
		
		$conf_obj = \aquagear\holder\ConfigureHolder::load($stack_file_path);
		$application_stack = $conf_obj->getAllData();
		foreach ($application_stack as $key => $class_name) {
			self::pushApplicationStackObject($key, $class_name);
		}
		
		if(\aquagear\application\ApplicationData::getAquagearServerEnv('cache') !== 'on' && 
			!empty(self::$_application_stack)) {
			self::$_cache_str = serialize(self::$_application_stack);
		}
		
		\aquagear\log\Logger::getLogInstance()->debug('end init application stack.' . "stack = " .var_export(self::$_application_stack, true));
	}
	
	/**
	 * 実行の最初のメソッド.
	 * 
	 * @return void
	 */
	public static function execute() {
		\aquagear\log\Logger::getLogInstance()->info('---------- start application --------------');
		\aquagear\log\Logger::getLogInstance()->info('exec uri = ' . \aquagear\holder\HttpParameterHolder::getInstance()->getURI(true));

		self::_initApplication();
		
		if(empty(self::$_application_stack)) {
			throw new \aquagear\exception\ApplicationException('empty application stack.', '500');
		}
		
		$stack = self::$_application_stack;
		foreach($stack as $obj) {
			\aquagear\log\Logger::getLogInstance()->debug('start execute class = ' . get_class($obj));
			if($obj->execute() === false) {
				\aquagear\log\Logger::getLogInstance()->warning('failed execute command class.');
				\aquagear\utility\HttpUtility::errorRedirect();
			}
			
			\aquagear\log\Logger::getLogInstance()->debug('end class = ' . get_class($obj));
		}
		
		self::_finalizeApplication();
	}
	
	/**
	 * ハンドラーのセット.
	 * 
	 * @return void
	 */
	private static function _setHandler() {
		\aquagear\handler\PhpErrorHandler::initHandler();
		\aquagear\handler\ExceptionHandler::initHandler();
		\aquagear\handler\ShutdownHandler::initHandler();
		\aquagear\handler\SessionHandler::initHandler();
	}
	
	/**
	 * アプリケーションスタックのデータの取得
	 * 
	 * @param string $key キー
	 * 
	 * @return obj コマンドオブジェクト
	 */
	public static function getApplicationStackObject($key) {
		return self::$_application_stack[$key];
	}
	
	/**
	 * スタックにコマンドオブジェクトをプッシュする
	 * 
	 * @param string $key キー
	 * @param string $class_name クラス名
	 * 
	 * @return void
	 */
	public static function pushApplicationStackObject($key, $class_name) {
		\aquagear\log\Logger::getLogInstance()->debug('start class = ' . $class_name);
		$obj = new $class_name;
		if($obj instanceof \aquagear\command\CoreCommand) {
			\aquagear\log\Logger::getLogInstance()->debug('start init class = ' . $class_name);
			if($obj->init()) {
				self::$_application_stack[$key] = $obj;
			}
		}
		else {
			throw new \aquagear\exception\ApplicationException('not extend \aquagear\command\CoreCommand.', '500');
		}
	}
	
	/**
	 * スタックからコマンドオブジェクトを削除する
	 *
	 * @param string $key スタックオブジェクトのキー
	 * 
	 * @return 成功時true,失敗時false
	 */
	public static function deleteApplicationStackObject($key) {
		if(isset(self::$_application_stack[$key])) {
			unset(self::$_application_stack[$key]);
			
			return true;
		}
		else {
			return false;
		}
	}
	
	/**
	 * スタックをクリアする
	 * 
	 * return void
	 */
	public static function allClearStackObject() {
		self::$_application_stack = array();
	}
}
?>