<?php 
namespace aquagear\auth;

/**
 * データベースでの認証のロジッククラス.
 * 
 * @package aquagear
 * @subpackage auth
 * 
 * @author motonari
 * @version $Id$
 *
 */
class DatabaseAuthentication extends CoreAuthentication {
	/**
	 * 認証用SQLパス.
	 * 
	 * @var string
	 */
	private $_conf_section = null;
	
	/**
	 * エラー時のリダイレクト.
	 * 
	 * @var string
	 */
	private $_error_redirect = null;
	
	/**
	 * 初期化メソッド.
	 * 
	 * @param obj $conf_obj 設定ファイルオブジェクト
	 * @return bool スタックにのせる場合true
	 */
	public function init($conf_obj=null) {
		$this->_conf_section = $conf_obj->getValue('section');
		
		return true;
	}
	
	/**
	 * 実行メソッド.
	 * 
	 * @return array 認証結果、認証失敗時はfalse
	 */
	public function execute() {
		$model_instance = \aquagear\model\Model::getModelInstance('pdo');
		$auth_data = $model_instance->getValue($this->_conf_section, array('id' => \aquagear\holder\HttpParameterHolder::getInstance()->getPostValue('id'),
																				'password' => \aquagear\holder\HttpParameterHolder::getInstance()->getPostValue('password')));
		if(empty($auth_data)) {
			return false;
		}
		
		return $auth_data[0];
	}
}
?>