<?php 
namespace aquagear\command;

/**
 * アクセス分析用コマンドクラス.
 * 
 * @package aquagear
 * @subpackage command
 * 
 * @author motonari
 * @version $Id$
 */
class AccessAnalysisCommand extends CoreCommand {

	/**
	 * 設定ファイルオブジェクト.
	 * 
	 * @var obj
	 */
	private $_conf_obj = null;
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/command/\aquagear\command\CoreCommand#init()
	 */
	public function init() {
		$configure_path = \aquagear\application\ApplicationData::getAquagearServerEnv('system_configure_root') . '/analysis.ini';
		if(is_file($configure_path)) {
			$this->_conf_obj = \aquagear\holder\ConfigureHolder::load($configure_path);
			if($this->_conf_obj->getValue('analysis') === false) {
				return false;
			}
		}
		else {
			return false;
		}
		
		return true;
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/command/\aquagear\command\CoreCommand#execute()
	 */
	public function execute() {
		$data = array();
		$data['date'] = date('Y/m/d H:i:s');
		$data['uri'] = \aquagear\holder\HttpParameterHolder::getInstance()->getServerValue('REQUEST_URI');
		$data['protocol'] = \aquagear\holder\HttpParameterHolder::getInstance()->getServerValue('SERVER_PROTOCOL');
		$data['ip'] = \aquagear\holder\HttpParameterHolder::getInstance()->getServerValue('REMOTE_ADDR');
		$data['port'] = \aquagear\holder\HttpParameterHolder::getInstance()->getServerValue('REMOTE_PORT');
		$data['agent'] = \aquagear\holder\HttpParameterHolder::getInstance()->getServerValue('HTTP_USER_AGENT');
		$data['reffer'] = \aquagear\holder\HttpParameterHolder::getInstance()->getServerValue('HTTP_REFERER');
		
		if($this->_conf_obj->getValue('path') === false) {
			return false;
		}
		
		\aquagear\file\FileReaderWriter::write($this->_conf_obj->getValue('path'), implode("\t", $data) . "\n", FILE_APPEND | LOCK_EX);
		return true;
	}
}
?>