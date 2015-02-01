<?php 
namespace aquagear\cache;

/**
 * ファイルキャッシュ.
 * 
 * @package aquagear
 * @subpackage cache
 * 
 * @author motonari
 * @version $Id: FileCache.inc 536 2011-07-26 14:58:17Z motonari $
 */
class FileCache extends Cache implements ICache {
	/**
	 * 設定のデフォルトのttlの値.
	 * 
	 * @var int
	 */
	private $_default_ttl = 0;
	
	/**
	 * プリフィックス.
	 * 
	 * @var string
	 */
	private $_prefix = '';
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/cache/\aquagear\cache\ICache#initCache()
	 */
	public function initCache(array $conf_arr) {
		if(! empty($conf_arr['ttl'])) {
			$this->_default_ttl = $conf_arr['ttl'];
		}
		
		if(! empty($conf_arr['prefix'])) {
			$this->_prefix      = $conf_arr['prefix'];
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
	 * @see aquagear/cache/\aquagear\cache\ICache#getValue()
	 */
	public function getValue($key) {
		if(is_file(\aquagear\application\ApplicationData::getAquagearServerEnv('cache_root') . '/' . $key)) {
			\aquagear\log\Logger::getLogInstance()->debug("get file cache [key=$this->_prefix . $key]");
			return \aquagear\file\FileReaderWriter::readByString(\aquagear\application\ApplicationData::getAquagearServerEnv('cache_root') . '/' . $this->_prefix . $key, 0, true);
		}
		
		return false;
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/cache/\aquagear\cache\ICache#setValue()
	 */
	public function setValue($key, $value, $ttl=0) {
		$_ttl = $this->_getTTL($ttl);
		
		if($this->cacheExists($key, $_ttl)) {
			return true;
		}
		
		$dir_path = \aquagear\application\ApplicationData::getAquagearServerEnv('cache_root') . "/" . dirname($key) . "/";
		if(! is_dir($dir_path)) {
			\aquagear\log\Logger::getLogInstance()->debug("make dir [dir_path = ". $dir_path ."]");
			\aquagear\file\FileOperator::mkdir($dir_path, 0777, true, true);
		}

		\aquagear\log\Logger::getLogInstance()->debug("write file cache [key=" . $this->_prefix . $key . "][ttl=$_ttl]");
		\aquagear\log\Logger::getLogInstance()->info('write file cache.');
		return \aquagear\file\FileReaderWriter::write(\aquagear\application\ApplicationData::getAquagearServerEnv('cache_root') . '/' . $this->_prefix . $key, $value, LOCK_EX, false);
	}

	/**
	 * (non-PHPdoc)
	 * @see aquagear/cache/\aquagear\cache\ICache#deleteValue($key)
	 */
	public function deleteValue($key) {
		$dir_path = \aquagear\application\ApplicationData::getAquagearServerEnv('cache_root') . "/" . dirname($key) . "/";
		if(is_file($dir_path . '/' . $this->_prefix . $key)) {
			return \aquagear\file\FileOperator::rm($dir_path . '/' . $this->_prefix . $key);
		}
		
		return false;
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/cache/\aquagear\cache\ICache#cacheExists()
	 */
	public function cacheExists($key, $ttl=0) {
		$_ttl = $this->_getTTL($ttl);
		
		if( is_file(\aquagear\application\ApplicationData::getAquagearServerEnv('cache_root') . '/' . $this->_prefix . $key) &&
			empty($_ttl)) {
			return true;
		}
		
		if(is_file(\aquagear\application\ApplicationData::getAquagearServerEnv('cache_root') . '/' . $this->_prefix . $key) &&
			$_ttl > time() - \aquagear\file\FileOperator::getMtime(\aquagear\application\ApplicationData::getAquagearServerEnv('cache_root') . '/' . $this->_prefix . $key) ) {
			return true;
		}
		
		return false;
	}
	
	/**
	 * TTLの取得.
	 * 
	 * @param int $ttl ttl
	 * @return int
	 */
	private function _getTTL($ttl) {
		if(empty($ttl)) {
			$ttl = $this->_default_ttl;
		}
		
		return $ttl;
	}
}
?>