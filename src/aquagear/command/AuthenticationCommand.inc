<?php
namespace aquagear\command;

/**
 * 認証コマンドクラス.
 * 
 * @package aquagear
 * @subpackage command
 * 
 * @author motonari
 * @version $Id: ControlCommand.inc 360 2009-09-03 15:05:53Z motonari $
 *
 */ 
class AuthenticationCommand extends CoreCommand {
	/**
	 * 設定ファイルオブジェクト.
	 * 
	 * @var object
	 */
	private $_conf_obj = null;
	
	/**
	 * 認証結果.
	 * 
	 * @var object
	 */
	public $auth_result = null;
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/command/\aquagear\command\CoreCommand#init()
	 */
	public function init() {
		$conf_path = \aquagear\application\ApplicationData::getAquagearServerEnv('system_configure_root') ."/authentication.ini"; 
		if(is_file($conf_path)) {
			$this->_conf_obj = \aquagear\holder\ConfigureHolder::load($conf_path);
		}
		
		return true;
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/command/\aquagear\command\CoreCommand#execute()
	 */
	public function execute() {
		$class_name = "";
		
		//XXX session value
		if(!empty($_SESSION['auth'])) {
			$this->auth_result = $_SESSION['auth'];
			return true;
		}
		
		if($this->_conf_obj &&
			$this->_conf_obj->getValue('class_name')) {
			$class_name = $this->_conf_obj->getValue('class_name');
		}
		
		if(! class_exists($class_name)) {
			\aquagear\log\Logger::getLogInstance()->debug('cannot exists class [ $class_name = ' . $class_name . ' ]');
			return false;
		}
		
		\aquagear\log\Logger::getLogInstance()->debug("execute  [class_name = $class_name]");
		$auth_obj = new $class_name;
		if($auth_obj instanceof \aquagear\auth\CoreAuthentication) {
			$auth_obj->init($this->_conf_obj);
			$this->auth_result = $auth_obj->execute();
			if($this->auth_result === false) {
				\aquagear\utility\HttpUtility::redirect($this->_conf_obj->getValue('error_redirect'));
			}
		}
		else {
			throw new \aquagear\exception\CommandException('not Authentication Object.');
		}
		
		$_SESSION['auth'] = $this->auth_result;
		return true;
	}

	/**
	 * ログアウト.
	 * 
	 * @return void
	 */
	public function logout() {
		unset($_SESSION['auth']);
	}

}
?>