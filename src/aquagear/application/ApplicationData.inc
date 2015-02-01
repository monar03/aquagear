<?php 
namespace aquagear\application;

/**
 * システム全体で使用するホルダークラス.
 * 
 * @package aquagear
 * @subpackage holder
 * 
 * @author motonari
 * @version $Id$
 *
 */
class ApplicationData extends \aquagear\holder\CoreHolder {
	/**
	 * システムの設定ファイルオブジェクト.
	 * 
	 * @var obj
	 */
	private static $_system_conf_obj = null;
	
	/**
	 * コンストラクタ.
	 * 
	 * @return void
	 */
	private function __construct() {
		
	}
	
	/**
	 * get system configure.
	 * 
	 * @return \aquagear\holder\ConfigureHolder 設定ファイルオブジェクト
	 */
	public static function getSystemConfigure($section) {
		if(self::$_system_conf_obj === null) {
			self::$_system_conf_obj = \aquagear\holder\ConfigureHolder::load(\aquagear\application\ApplicationData::getAquagearServerEnv('system_configure_root')."/system.ini", true);
		}
		
		return self::$_system_conf_obj->getValue($section);
	}
	
	/**
	 * サーバーで設定を行っている、環境変数の取得を行う。
	 * 
	 * 使用できるキーは以下の通り
	 * <code>
	 * debug : デバッグ設定
	 * cache : 設定ファイルなどのキャッシュの設定
	 * system_configure_root : フレームワーク全体の設定ファイルのルートパス
	 * application_configure_root : URIごとに使用する設定ファイルのルートパス
	 * template_root : テンプレートファイルのルートパス
	 * cache_root : キャッシュのルートパス
	 * model_root : モデルの設定ファイルのルートパス
	 * </code>
	 * 
	 * @param string $key キー
	 * 
	 * @return string
	 */
	public static function getAquagearServerEnv($key) {
		static $server_env = null;
		
		if($server_env === null) {
			$server_env = array(
									'cache'                      => \aquagear\holder\HttpParameterHolder::getInstance()->getServerValue('AQUAGEAR_CACHE'),
									'system_configure_root'      => \aquagear\holder\HttpParameterHolder::getInstance()->getServerValue('AQUAGEAR_SYSTEM_CONFIGURE_ROOT'),
									'application_configure_root' => \aquagear\holder\HttpParameterHolder::getInstance()->getServerValue('AQUAGEAR_APPLICATION_CONFIGURE_ROOT'),
									'control_root'               => \aquagear\holder\HttpParameterHolder::getInstance()->getServerValue('AQUAGEAR_CONTROL_ROOT'),
									'template_root'              => \aquagear\holder\HttpParameterHolder::getInstance()->getServerValue('AQUAGEAR_TEMPLATE_ROOT'),
									'cache_root'                 => \aquagear\holder\HttpParameterHolder::getInstance()->getServerValue('AQUAGEAR_CACHE_ROOT'),
									'model_root'                 => \aquagear\holder\HttpParameterHolder::getInstance()->getServerValue('AQUAGEAR_MODEL_ROOT')
								);
		}

		if(isset($server_env[$key])) {
			return $server_env[$key];
		}
		else {
			return false;
		}
	}
	
	/**
	 * アプリケーションの設定ファイルのdirectoryを返す.
	 * 
	 * @return string
	 */
	public static function getApplicationConfDir() {
		$conf_dir = \aquagear\holder\HttpParameterHolder::getInstance()->getURI();
		
		$base_configure = \aquagear\application\ApplicationData::getSystemConfigure('base');
		$extension = $base_configure['app_conf_dir_extension'];
		if($extension) {
			$conf_dir .= $extension;
		}
		
		return \aquagear\application\ApplicationData::getAquagearServerEnv('application_configure_root') . $conf_dir;
	}
}
?>