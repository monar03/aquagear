<?php 
namespace aquagear\holder;

/**
 * cookie用ホルダークラス.
 * 
 * @package aquagear
 * @subpackage holder
 * 
 * @author motonari
 * @version $Id$
 *
 */
class CookieHolder extends CoreHolder {
	/**
	 * コンストラクタ.
	 * 
	 * @return void
	 */
	private function __construct() {
		
	}
	
	/**
	 * 自オブジェクトの取得.
	 * 
	 * @return object 自インスタンス
	 */
	public static function getInstance() {
		static $instance = null;
		
		if($instance === null) {
			$instance = new \aquagear\holder\CookieHolder();
		}
		
		return $instance;
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/\aquagear\Object#__set()
	 */
	public function __set($arg, $value) {
		$this->setValue($arg, $value);
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/\aquagear\Object#__get()
	 */
	public function __get($arg) {
		return $this->getValue($arg);
	}
	
	/**
	 * データのセット.
	 * 
	 * @param string $arg 添え字
	 * @param mixed $value 代入値
	 * @return void
	 */
	public function setValue($arg, $value) {
		\aquagear\log\Logger::getLogInstance()->debug("set cookie value [arg=$arg][value=$value]");
		setcookie($arg, $value);
	}

	/**
	 * データの取得.
	 * 
	 * @param string $arg 添え字
	 * @param int $filter フィルターの定義値
	 * 
	 * @return string
	 */
	public function getValue($arg, $default=false, $filter=parent::NO_FILTER) {
		if(isset($_COOKIE[$arg])) {
			\aquagear\log\Logger::getLogInstance()->debug("get cookie value [arg=$arg][value=".$_COOKIE[$arg]."]");
			return parent::filterValue($_COOKIE[$arg], $filter);
		}
		
		\aquagear\log\Logger::getLogInstance()->debug("get data value [arg=$arg][value=]");
		return $default;		
	}
	
	/**
	 * すべてのデータの取得.
	 * 
	 * @return array
	 */
	public function getAllData() {
		\aquagear\log\Logger::getLogInstance()->debug("get all data value. " . var_export($_COOKIE, true));
		return $_COOKIE;
	}
}
?>