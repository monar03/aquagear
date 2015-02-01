<?php
namespace aquagear\model;

/**
 * PDOのデータベースコネクタ.
 * 
 * @package aquagear 
 * @subpackage model
 * 
 * @author motonari
 * @version $Id: PDODatabaseConnector.inc 536 2011-07-26 14:58:17Z motonari $
 */
class PDODatabaseConnector extends Model implements IModelConnector {
	/**
	 * モデルのインスタンス.
	 * 
	 * @var object
	 */
	private $_model_instance = null;
	
	/**
	 * モデルの設定ファイル.
	 * 
	 * @var array
	 */
	private $_model_system_conf = null;
	
	/**
	 * SQL
	 * @var string
	 */
	private $_sql_str = "";
	
	/**
	 * データのキャッシュ
	 * 
	 * @var array
	 */
	private $_cache_data = array();
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/model/\aquagear\model\IModelConnector#initConnection()
	 */
	public function initConnection(array $conf_arr) {
		$this->_model_system_conf = $conf_arr;
		
		$this->_model_instance = new \PDO($this->_model_system_conf['dsn'], 
											$this->_model_system_conf['user'],
											$this->_model_system_conf['password']);
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
	 * @see aquagear/model/\aquagear\model\IModelConnector#getValue($conf_section, $param)
	 */
	public function getValue($conf_section, array $param=array()) {
		if(isset($this->_cache_data[md5($conf_section . var_export($param, true))])) {
			return $this->_cache_data[md5($conf_section . var_export($param, true))];
		}
		
		$section_conf_arr = $this->getApplictionConf($conf_section);
		if(empty($section_conf_arr)) {
			throw new \aquagear\exception\ModelException('not exists this section. ['.$section.']', '500');
		}
		
		$this->setSQLFromFile($section_conf_arr['source'], $param);
		\aquagear\log\Logger::getLogInstance()->debug("prepear sql.\n sql = ".$this->_sql_str );
		
		$sql_statement = $this->_executeStatement($param, $sql_flag);
		if(!$sql_flag) {
			throw new \aquagear\exception\ModelException('can not execute sql. sql=' . $this->_sql_str . "\n holder = " . var_export($param, true), '500');
		}
		
		$data = $sql_statement->fetchAll(\PDO::FETCH_ASSOC);
		$this->_cache_data[md5($conf_section . var_export($param, true))] = $data;
		
		return $this->_cache_data[md5($conf_section . var_export($param, true))];
	}

	/**
	 * (non-PHPdoc)
	 * @see aquagear/model/\aquagear\model\IModelConnector#setValue($conf_section, $param)
	 */
	public function setValue($conf_section, array $param) {
		$section_conf_arr = $this->getApplictionConf($conf_section);
		if(empty($section_conf_arr)) {
			throw new \aquagear\exception\ModelException('not exists this section. ['.$section.']', '500');
		}
		
		$this->setSQLFromFile($section_conf_arr['source'], $param);
		\aquagear\log\Logger::getLogInstance()->debug("prepear sql.\n sql = ".$this->_sql_str );

		$this->_executeStatement($param, $sql_flg);
		if(!$sql_flg) {
			throw new \aquagear\exception\ModelException('can not execute sql. sql=' . $this->_sql_str . "\n holder = " . var_export($param, true), '500');
		}
		
		return $sql_flag;
	}

	/**
	 * SQL statement処理
	 * 
	 * @param array $param　パラメータホルダ
	 * @param bool $sql_flag SQLの成功失敗
	 * 
	 * @return PDOStatement 処理後のステートメント
	 */
	private function _executeStatement(array $param, &$sql_flag=true) {
		$sql_statement = $this->_model_instance->prepare($this->_sql_str);
		if($sql_statement === false) {
			\aquagear\log\Logger::getLogInstance()->warning('create statement error.' . var_export($this->_model_instance->errorInfo(), true));
			return false;
		}
		
		if($param !== null) {
			foreach($param as $key => $value) {
				$this->_bindValue($sql_statement, $key, $value);
			}
		}
		
		$sql_flag = $sql_statement->execute();
		if(! $sql_flag) {
			\aquagear\log\Logger::getLogInstance()->info('pdo error = ' . var_export($sql_statement->errorInfo(), true));
		}
		return $sql_statement;
	}
	
	/**
	 * ファイルから、SQLのセット.
	 * 
	 * @param string $file_path ファイルパス
	 * 
	 * @return void
	 */
	private function setSQLFromFile($file_path, array $param) {
		$sql_path = $this->_model_system_conf['sql_root'] . '/' . $file_path;

		if(is_file($sql_path)) {
			ob_start();
			ob_implicit_flush(0);
			include($sql_path);
			$this->_sql_str =  ob_get_clean();
		}
	}
	
	/**
	 * プレースホルダーにバインドを行う.
	 * 
	 * @param object $sql_statement プリペアステートメント
	 * @param string $key キー
	 * @param mixed $value バインドする値
	 * @return bool
	 */
	private function _bindValue($sql_statement, $key, $value) {
		$ret = false;
		
		if(is_int($value)) {
			\aquagear\log\Logger::getLogInstance()->debug("bind param int escape key=" . $key . " value=" . $value);
			$ret = $sql_statement->bindValue($key, $value, \PDO::PARAM_INT);
		}
		else if(is_bool($value)) {
			\aquagear\log\Logger::getLogInstance()->debug("bind param bool escape key=" . $key . " value=" . $value);
			$ret = $sql_statement->bindValue($key, $value, \PDO::PARAM_BOOL);
		}
		else if($value === null) {
			\aquagear\log\Logger::getLogInstance()->debug("bind param null escape key=" . $key . " value=" . $value);
			$ret = $sql_statement->bindValue($key, $value, \PDO::PARAM_NULL);
		}
		else {
			\aquagear\log\Logger::getLogInstance()->debug("bind param string escape key=" . $key . " value=" . $value);
			$ret = $sql_statement->bindValue($key, $value, \PDO::PARAM_STR);
		}
		
		return $ret;
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/model/\aquagear\model\IModelConnector#deleteValue($conf_section, $param)
	 */
	public function deleteValue($conf_section, array $param=array()) {
		$section_conf_arr = $this->getApplictionConf($conf_section);
		if(empty($section_conf_arr)) {
			throw new \aquagear\exception\ModelException('not exists this section. ['.$section.']', '500');
		}
		
		$this->setSQLFromFile($section_conf_arr['source'], $param);
		\aquagear\log\Logger::getLogInstance()->debug("prepear sql.\n sql = ".$this->_sql_str );

		$this->_executeStatement($param, $sql_flg);
		if(!$sql_flg) {
			throw new \aquagear\exception\ModelException('can not execute sql. sql=' . $this->_sql_str . "\n holder = " . var_export($param, true), '500');
		}
		
		return $sql_flag;
	}
	
	
	/**
	 * トランザクションの開始.
	 * 
	 * @return bool
	 */
	public function beginTransaction() {
		return $this->_model_instance->beginTransaction();
	}
	
	/**
	 * ロールバック.
	 * 
	 * @return bool
	 */
	public function rollback() {
		return $this->_model_instance->rollback();
	}
}