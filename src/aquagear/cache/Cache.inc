<?php
namespace aquagear\cache;

/** 
 * Cacheのインスタンスの取得用クラス.
 * 
 * @package aquagear
 * @subpackage cache
 * 
 * @author motonari
 * @version $Id: Cache.inc 536 2011-07-26 14:58:17Z motonari $
 *
 */
class Cache extends \aquagear\Object {
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
	 * キャッシュ用インスタンスの取得.
	 * 
	 * @param mixed $property プロパティ値
	 * @return object
	 */
	public static function getCacheInstance($type) {
		
		if(! isset(self::$_instance[$type])) {
			//TODO 設定ファイルのキャッシュはいらないかも.
			if($type === 'conf') {
				self::$_instance[$type] = new \aquagear\cache\ConfigureCache(array());
			}
			else {
				self::$_instance[$type] = self::_initCache(self::_getCacheConfigureArr($type));
			}
		}
		
		return self::$_instance[$type];
	}
	
	/**
	 * キャッシュのインスタンスの初期化.
	 * 
	 * @param mixed $property プロパティ値
	 * @return object
	 */
	private static function _initCache($conf_arr) {
		$instance = new $conf_arr['class_name'];
		if($instance instanceof \aquagear\cache\ICache) {
			$instance->initCache($conf_arr);
		}
		else {
			throw new \aquagear\exception\CacheException('not extends CoreCache.', '500');
		}
		
		return $instance;
	}
	
	/**
	 * キャッシュのセクションの設定を取得する.
	 * 
	 * @param string $section セクション名
	 * @return array
	 */
	private static function _getCacheConfigureArr($section) {
		$conf_obj = \aquagear\holder\ConfigureHolder::load(\aquagear\application\ApplicationData::getAquagearServerEnv('system_configure_root') . "/cache.ini", true);
		
		return $conf_obj->getValue($section);
	}
}
?>