<?php
namespace aquagear\cache;

/** 
 * APCキャッシュの場合のクラス.
 * 
 * @package aquagear
 * @subpackage cache
 * 
 * @author motonari
 * @version $Id: ApcCache.inc 536 2011-07-26 14:58:17Z motonari $
 *
 */
class ApcCache extends Cache implements ICache {
	/**
	 * デフォルトのTTL.
	 * 
	 * @var int
	 */
	private $_default_ttl = 0;
	
	/**
	 * キャッシュのキーのprefix.
	 * 
	 * @var string
	 */
	private $_prefix = "";
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/cache/\aquagear\cache\ICache#initCache()
	 */
	public function initCache(array $conf_arr) {
		if(! empty($conf_arr['ttl'])) {
			$this->_default_ttl = $conf_arr['ttl'];
		}
		
		if(! empty($conf_arr['prefix'])) {
			$this->_prefix = $conf_arr['prefix'];
		}
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
	 * @see aquagear/cache/\aquagear\cache\ICache#setValue()
	 */
	public function setValue($key, $value, $ttl=0) {
		$ret = true;
		if($ttl === 0) {
			$ttl = $this->_default_ttl;
		}
		
		if($this->getValue($key) === false) {
			\aquagear\log\Logger::getLogInstance()->debug("write apc cache [key=$key]");
			$ret = apc_store($this->_prefix . $key, $value, $ttl);
		}
		
		return $ret;
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/cache/\aquagear\cache\ICache#deleteValue($key)
	 */
	public function deleteValue($key) {
		return apc_delete($this->_prefix . $key);
	}
	
	/** 
	 * (non-PHPdoc)
	 * @see aquagear/cache/\aquagear\cache\ICache#getValue()
	 */
	public function getValue($key) {
		\aquagear\log\Logger::getLogInstance()->debug("get apc cache [key=$key]");
		return apc_fetch($this->_prefix . $key);
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/cache/\aquagear\cache\ICache#cacheExists()
	 */
	public function cacheExists($key, $ttl=0) {
		return apc_fetch($this->_prefix . $key);
	}
}
?>