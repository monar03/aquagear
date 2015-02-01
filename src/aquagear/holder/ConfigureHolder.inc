<?php
namespace aquagear\holder;

/**
 * 設定ファイルのホルダークラス.
 * 
 * @package aquagear
 * @subpackage holder
 * 
 * @author motonari
 * @version $Id: ConfigureHolder.inc 538 2011-07-31 12:52:00Z motonari $
 */
class ConfigureHolder extends CoreHolder {
	/**
	 * 設定ファイルの値を入れる.
	 * 
	 * @var array
	 */
	private $_data = array();
	
	/**
	 * コンストラクタ
	 * 
	 * @param string $path_str 設定ファイルのパス
	 * @param bool $section セクションのon,off
	 * 
	 * @return void
	 */
	private function __construct($path_str, $section=false) {
		$this->_loadConfigure($path_str, $section);
	}
	
	/**
	 * 設定ファイルをロードする.
	 * 
	 * @param string $path_str 設定ファイルのパス
	 * @param bool $section セクションのon,off
	 * 
	 * @return \aquagear\holder\ConfigureHolder 設定ファイルオブジェクト
	 */
	public static function load($path_str, $section=false) {
		static $load_configure = array();
		
		if(isset($load_configure[md5($path_str)])) {
			return $load_configure[md5($path_str)];
		}
		else {
			$instance = unserialize(\aquagear\cache\Cache::getCacheInstance('conf')->getValue($path_str));
			if(empty($instance) || \aquagear\application\ApplicationData::getAquagearServerEnv('cache') !== 'on') {
				$instance = new \aquagear\holder\ConfigureHolder($path_str, $section);
				\aquagear\cache\Cache::getCacheInstance('conf')->setValue($path_str, serialize($instance));
			}

			$load_configure[md5($path_str)] = $instance;
			return $instance;
		}
	}
	
	/**
	 * 設定ファイルの読み込み.
	 * 
	 * @param string $path_str 設定ファイルのパス
	 * @param bool $section セクションのon,off
	 * 
	 * @return bool 成功の場合true
	 */
	private function _loadConfigure($path_str, $section=false) {
		if(is_file($path_str)) {
			$this->_data = parse_ini_file($path_str, $section);
			if($this->_data === false) {
				throw new \aquagear\exception\ConfigureException("Cannot read configure file [configure path = $path_str][section = $section]", '500');
			}
		}
		else {
			throw new \aquagear\exception\ConfigureException("Cannot find configure file [configure path = $path_str][section = $section]", '500');
		}
		
		return true;
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/\aquagear\Object#__get()
	 */
	public function __get($arg) {
		return $this->getValue($arg);
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/\aquagear\Object#__set()
	 */
	public function __set($arg, $value) {
		
	}
	
	/**
	 * 値の取得.
	 * 
	 * @param string $arg キー
	 * @param string $filter フィルタ定義値
	 * 
	 * @return mixed
	 */
	public function getValue($arg, $default=false, $filter=parent::NO_FILTER) {
		if(isset($this->_data[$arg])) {
			return parent::filterValue($this->_data[$arg], $filter);
		}
		
		return $default;
	}
	
	/**
	 * すべての値の取得.
	 * 
	 * return array
	 */
	public function getAllData() {
		return $this->_data;
	}
}