<?php 
namespace aquagear\log;

/**
 * ログクラスの呼び出しようクラス.
 * 
 * @package aquagear
 * @subpackage log
 * 
 * @author motonari
 * @version $Id: Logger.inc 535 2011-07-26 14:02:27Z motonari $
 */
class Logger extends \aquagear\Object {
	/**
	 * インスタンス.
	 * 
	 * @var array
	 */
	private static $_instance = array();

	/**
	 * コンストラクタ.
	 * 
	 * @return void
	 */
	protected function __construct() {
		
	}
	
	/**
	 * 自インスタンスの取得.
	 * 
	 * @param mixed $property 制御値.
	 * @return object ログインスタンス.
	 */
	public static function getLogInstance($section='default') {
		if(! isset(self::$_instance[$section])) {
			self::$_instance[$section] = self::_initLogger($section);
		}

		return self::$_instance[$section];
	}
	
	/**
	 * ログクラスの初期化.
	 * 
	 * @param mixed $property 設定値
	 * @return object ログクラス
	 */
	private static function _initLogger($section) {
		$conf_arr = \aquagear\holder\ConfigureHolder::load(\aquagear\application\ApplicationData::getAquagearServerEnv('system_configure_root') . "/logger.ini", true)->getValue($section);

		$instance = new $conf_arr['class_name'];
		if($instance instanceof \aquagear\log\ILogger) {
			$instance->initLogger($conf_arr);
		}
		else {
			throw new \aquagear\exception\LoggerException('not extends CoreLogger.', '500');
		}
		
		return $instance;
	}
	
	/**
	 * 終了処理
	 */
	public static function finalize() {
		foreach(self::$_instance as $obj) {
			$obj->__destruct();
		}
	}
}
?>