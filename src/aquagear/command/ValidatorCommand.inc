<?php
namespace aquagear\command;

/**
 * バリデイト用コマンドクラス.
 * 
 * @package aquagear
 * @subpackage command
 * 
 * @author motonari
 * @version $Id: ValidatorCommand.inc 536 2011-07-26 14:58:17Z motonari $
 */ 
class ValidatorCommand extends CoreCommand {
	/**
	 * バリデイトの設定の配列.
	 * 
	 * @var array
	 */
	private $_validate_conf = null;
	
	/**
	 * バリデイトの結果.
	 * 
	 * @var array
	 */
	private $_validate_data = null;
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/command/\aquagear\command\CoreCommand#init()
	 */
	public function init() {
		$ret = false;
		$conf_path = \aquagear\application\ApplicationData::getApplicationConfDir() . "/validate.ini";
		
		if(is_file($conf_path)) {
			$this->_validate_conf = array();
			
			$conf_obj = \aquagear\holder\ConfigureHolder::load($conf_path, true);
			$conf_arr = $conf_obj->getAllData();
			foreach($conf_arr as $key => $value) {
				$this->_validate_conf[$key] = $this->_parseValidateConf($value);
			}
			
			$ret = true;
		}
		
		return $ret;
	}
	
	/**
	 * セクションごとの設定をパースする.
	 * 
	 * @param array $conf_arr セクションの設定配列.
	 * 
	 * @return array パース結果
	 */
	private function _parseValidateConf($conf_arr) {
		$data_arr = array();
		foreach($conf_arr as $key => $value) {
			list($name, $type) = explode('.', $key, 2);
			$data_arr[$name][$type] = $value;
		}
		
		return $data_arr;
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/command/\aquagear\command\CoreCommand#execute()
	 */
	public function execute() {
		$error_validate_arr = array();
		$value = null;
		
		$validate_arr = $this->_validate_conf;
		if($validate_arr === null) {
			return false;
		}
		
		foreach($validate_arr as $key => $section_conf_arr) {
			foreach($section_conf_arr as $name => $conf_arr) {
				if($conf_arr['method'] === 'get') {
					$value = \aquagear\holder\HttpParameterHolder::getInstance()->getGetValue($key);
				}
				if($conf_arr['method'] === 'post') {
					$value = \aquagear\holder\HttpParameterHolder::getInstance()->getPostValue($key);
				}
				else {
					$value = \aquagear\holder\HttpParameterHolder::getInstance()->getFileValue($key);
				}

				$ret = \aquagear\utility\ClassUtility::callMethod($conf_arr['class_name'], 
																	$conf_arr['method_name'], 
																	array($value, $conf_arr['param']));
				if($ret === false) {
					$error_validate_arr[$key][$name] = $conf_arr['error_string'];
				}
				else if(is_string($ret)) {
					$error_validate_arr[$key][$name] = $ret;
				}
			}
		}
		
		$this->_validate_data = $error_validate_arr;
		return true;
	}

	/**
	 * バリデイトの結果の取得.
	 * 
	 * @return array
	 */
	public function getValidateData() {
		return $this->_validate_data;
	}
}
?>