<?php 
namespace aquagear\model;

/**
 * Fileモデルクラス
 * 
 * @package aquagear
 * @subpackage model
 * 
 * @author motonari
 * @version $Id$
 *
 */
class FileModelConnector extends Model implements IModelConnector {
	/**
	 * モデルの設定ファイル.
	 * 
	 * @var array
	 */
	private $_model_system_conf = null;
	
	/**
	 * ファイルDBのコントロールインスタンス
	 * 
	 * @var obj
	 */
	private $_file_db_instance = null;
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/model/\aquagear\model\IModelConnector#initConnection()
	 */
	public function initConnection(array $conf_arr) {
		$this->_model_system_conf = $conf_arr;
		$this->_file_db_instance = new \aquagear\db\FileDBModeler($this->_model_system_conf['base_dir']);
	}
	
	/**
	 * モデルインスタンスの作成.
	 * 
	 * @param string $section 設定ファイルのセクション名
	 * @return object
	 */
	public static function getModelInstance($section) {
		throw new \aquagear\exception\ModelException('can not execute '. __CLASS__ .'getModelInstance($section)', '500');
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/model/\aquagear\model\IModelConnector#setValue($conf_section, $param)
	 */
	public function setValue($conf_section, array $param) {
		$conf_arr = $this->getApplictionConf($conf_section);
		$file_path = $conf_arr['source'];
		
		if(isset($param['filename'])) {
			$file_path .= '/' . $param['filename'];
		}
		
		return $this->_file_db_instance->set($file_path, serialize($param['data']));
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/model/\aquagear\model\IModelConnector#getValue($conf_section, $param)
	 */
	public function getValue($conf_section, array $param=array()) {
		$conf_arr = $this->getApplictionConf($conf_section);
		$file_path = $conf_arr['source'];
		
		if(isset($param['filename'])) {
			$file_path .= '/' . $param['filename'];
		}
				
		return unserialize($this->_file_db_instance->get($file_path));
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/model/\aquagear\model\IModelConnector#deleteValue($conf_section, $param)
	 */
	public function deleteValue($conf_section, array $param=array()) {
		$conf_arr = $this->getApplictionConf($conf_section);
		$file_path = $conf_arr['source'];
		
		if(isset($param['filename'])) {
			$file_path .= '/' . $param['filename'];
		}
				
		return $this->_file_db_instance->delete($file_path);
	}
}
?>