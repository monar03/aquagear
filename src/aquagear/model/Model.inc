<?php 
namespace aquagear\model;

/**
 * モデルのfactoryクラス.
 * 
 * @package aquagear
 * @subpackage model
 * 
 * @author motonari
 * @version $Id: Model.inc 535 2011-07-26 14:02:27Z motonari $
 *
 */
class Model extends \aquagear\Object {
	/**
	 * インスタンス.
	 * 
	 * @var array
	 */
	private static $_instance = array();

	/**
	 * コンストラクタ.
	 * 
	 * @return void
	 */
	protected function __construct() {
		
	}
	
	/**
	 * モデルインスタンスの作成.
	 * 
	 * @param string $section 設定ファイルのセクション名
	 * @return object
	 */
	public static function getModelInstance($section) {
		
		if(! isset(self::$_instance[$section])) {
			self::$_instance[$section] = self::_initModel(self::_getModelConfigureArr($section));
		}
		
		return self::$_instance[$section];
	}
	
	/**
	 * モデルインスタンスの初期化.
	 * 
	 * @param array $conf_arr 設定ファイルの配列.
	 * @return object
	 */
	private static function _initModel($conf_arr) {		
		$instance = new $conf_arr['class_name'];
		if($instance instanceof \aquagear\model\IModelConnector) {
			$instance->initConnection($conf_arr);
		}
		else {
			throw new \aquagear\exception\ModelException('not extends CoreModel.', '500');
		}
		
		return $instance;
	}
	
	/**
	 * 設定ファイルの配列の取得.
	 * 
	 * @param string $section 設定ファイルのセクション名
	 * @return array
	 */
	private static function _getModelConfigureArr($section) {
		$conf_obj = \aquagear\holder\ConfigureHolder::load(\aquagear\application\ApplicationData::getAquagearServerEnv('system_configure_root') . "/model.ini" ,true);
				
		return $conf_obj->getValue($section);
	}
	
	/**
	 * モデルのセクションの配列を取得.
	 * 
	 * @param string $exec_path セクションパス
	 * 
	 * @return array セクションの配列
	 */
	protected function getApplictionConf($exec_path) {
		$data_arr = array();
		
		if(preg_match('/^http\:/', $exec_path) !== 0) {
			$data_arr['source'] = $exec_path;
			$data_arr['cache']  = false;
		}
		else if(preg_match('/^file\:/', $exec_path) !== 0) {
			list($head, $source) = explode(':', $exec_path, 2);
			$data_arr['source'] = $source;
			$data_arr['cache']  = false;
		}
		else {
			$path_arr = explode('/', rtrim($exec_path, '/'));
			$section = array_pop($path_arr);

			$private_configure_path  = \aquagear\application\ApplicationData::getAquagearServerEnv('model_root') . '/' . implode('/', $path_arr) . '/model.ini';
			$conf_arr = \aquagear\holder\ConfigureHolder::load($private_configure_path, true);
			
			$data_arr = $conf_arr->getValue($section);
		}
		
		return $data_arr;
	}
}
?>