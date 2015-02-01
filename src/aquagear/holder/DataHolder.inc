<?php 
namespace aquagear\holder;

/**
 * データ用ホルダークラス.
 * 
 * @package aquagear
 * @subpackage holder
 * 
 * @author motonari
 * @version $Id: DataHolder.inc 535 2011-07-26 14:02:27Z motonari $
 *
 */
class DataHolder extends CoreHolder {
	/**
	 * データリスト.
	 * 
	 * @var array
	 */
	private $_data = array();

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
			$instance = new \aquagear\holder\DataHolder();
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
		\aquagear\log\Logger::getLogInstance()->debug("set data value [arg=$arg][value=$value]");
		$this->_data[$arg] = $value;
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
		if(isset($this->_data[$arg])) {
			\aquagear\log\Logger::getLogInstance()->debug("get data value [arg=$arg][value=".$this->_data[$arg]."]");
			return parent::filterValue($this->_data[$arg], $filter);
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
		\aquagear\log\Logger::getLogInstance()->debug("get all data value. " . var_export($this->_data, true));
		return $this->_data;
	}
}
?>