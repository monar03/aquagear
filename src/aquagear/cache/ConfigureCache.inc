<?php
namespace aquagear\cache;

/**
 * 設定ファイル用キャッシュ.
 * 
 * @package aquagear
 * @subpackage cache
 * 
 * @author motonari
 * @version $Id: ConfigureCache.inc 536 2011-07-26 14:58:17Z motonari $
 * 
 * @todo このキャッシュに関してはいらないかも
 */ 
class ConfigureCache extends Cache implements ICache {
	/**
	 * (non-PHPdoc)
	 * @see aquagear/cache/\aquagear\cache\ICache#initCache()
	 */
	public function initCache(array $conf_arr) {
		
	}
	
	/**
	 * キャッシュ用インスタンスの取得.
	 * 
	 * @param mixed $property プロパティ値
	 * @return object
	 */
	public static function getCacheInstance($type) {
		throw new \aquagear\exception\CacheException('can not execute '. __CLASS__ .'getCacheInstance($type)', '500');
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/cache/\aquagear\cache\ICache#getValue()
	 */
	public function getValue($key) {
		if(function_exists('apc_fetch')) {
			return apc_fetch($key);
		}
		else {
			return false;
		}
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/cache/\aquagear\cache\ICache#setValue()
	 */
	public function setValue($key, $value, $ttl=0) {
		if(function_exists('apc_store')) {
			$ret = true;
		
			if($this->getValue($key) === false) {
				$ret = apc_store($key, $value, $ttl);
			}
			
			return $ret;
		}
		else {
			return false;
		}
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/cache/\aquagear\cache\ICache#deleteValue($key)
	 */
	public function deleteValue($key) {
		return true;
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/cache/\aquagear\cache\ICache#cacheExists()
	 */
	public function cacheExists($key, $ttl=0) {
		if(function_exists('apc_store')) {
			return apc_store($key, $value, $ttl);
		}
		else {
			return false;
		}
	}
}
?>